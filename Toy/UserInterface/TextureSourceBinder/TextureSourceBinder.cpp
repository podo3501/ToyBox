#include "pch.h"
#include "TextureSourceBinder.h"
#include "../JsonHelper.h"
#include "../../Utility.h"
#include "../UIUtility.h"

TextureSourceBinder::~TextureSourceBinder() = default;
TextureSourceBinder::TextureSourceBinder()
{}

bool TextureSourceBinder::LoadResources(ITextureLoad* load)
{
    return ranges::all_of(m_bindingTable, [load](auto& pair) {
        return pair.second.LoadResource(load);
        });
}

bool TextureSourceBinder::Load(const wstring& jsonFilename)
{
    ReturnIfFalse(JsonFile::Read(jsonFilename, *this));
    m_jsonFilename = jsonFilename;

    return true;
}

bool TextureSourceBinder::Save(const wstring& jsonFilename)
{
    ReturnIfFalse(JsonFile::Write(*this, jsonFilename));
    m_jsonFilename = jsonFilename;

    return true;
}

bool TextureSourceBinder::InsertBindingKey(const string& bindingKey, const TextureSourceInfo& sourceAreas) noexcept
{
    if (bindingKey.empty()) return true;
    if (auto it = m_bindingTable.find(bindingKey); it != m_bindingTable.end())
    {
        if (it->second != sourceAreas) return false; //같은 이름이 있다.
        return true;
    }

    m_bindingTable.emplace(bindingKey, sourceAreas);
    return true;
}

bool TextureSourceBinder::ModifyBindingKey(const string& newKey, const string& preKey) noexcept
{
    if (newKey.empty())
    {
        RemoveBindingKey(preKey);
        return true;
    }
    if (auto it = m_bindingTable.find(newKey); it != m_bindingTable.end()) return false;
    
    if (auto it = m_bindingTable.find(preKey); it != m_bindingTable.end())
    {
        TextureSourceInfo value = it->second;
        m_bindingTable.erase(it);
        m_bindingTable.emplace(newKey, value);
    }
    return true;
}

void TextureSourceBinder::RemoveBindingKey(const string& bindingKey) noexcept
{
    if (auto it = m_bindingTable.find(bindingKey); it != m_bindingTable.end())
        m_bindingTable.erase(it);
}

void TextureSourceBinder::RemoveBindingKey(const wstring& filename) noexcept
{
    erase_if(m_bindingTable, [&filename](const auto& pair) {
        return pair.second.filename == filename;
        });
}

vector<wstring> TextureSourceBinder::GetTextureFiles() const noexcept
{
    unordered_set<wstring> seenFilenames;

    for (const auto& entry : m_bindingTable) //중복 방지를 위해서 set에 넣음
        seenFilenames.insert(entry.second.filename);

    return vector<wstring>(seenFilenames.begin(), seenFilenames.end());
}

string TextureSourceBinder::GetBindingKey(const TextureSourceInfo& sourceAreas) const noexcept
{
    auto it = ranges::find_if(m_bindingTable, [&](const auto& pair) { return pair.second == sourceAreas; });
    return (it != m_bindingTable.end()) ? it->first : "";
}

Rectangle TextureSourceBinder::GetArea(const string& key, int index) const noexcept
{
    auto it = m_bindingTable.find(key);
    if (it == m_bindingTable.end()) return {};

    return it->second.GetSource(index);
}

vector<Rectangle> TextureSourceBinder::GetArea(const wstring& filename, ImagePart imgPart, const XMINT2& position) const noexcept
{
    for (const auto& entry : m_bindingTable) {
        const TextureSourceInfo& sourceInfo = entry.second;
        if (sourceInfo.filename != filename) continue;
        if (sourceInfo.imagePart != imgPart) continue;
        if (!IsContains(sourceInfo.sources, position)) continue;
        
        return sourceInfo.sources;
    }
    return {};
}

vector<TextureSourceInfo> TextureSourceBinder::GetAreas(const wstring& filename, ImagePart part) const noexcept
{
    vector<TextureSourceInfo> filteredTextures;
    for (const auto& entry : m_bindingTable) {
        const TextureSourceInfo& sourceInfo = entry.second;
        if (sourceInfo.filename == filename && sourceInfo.imagePart == part)
            filteredTextures.push_back(sourceInfo);
    }
    return filteredTextures;
}

pair<wstring, Rectangle> TextureSourceBinder::GetSourceInfo(const string& bindKey, size_t sourceIndex) const noexcept
{
    auto it = m_bindingTable.find(bindKey);
    if (it == m_bindingTable.end() || it->second.sources.size() <= sourceIndex) return {};

    return make_pair(it->second.filename, it->second.sources[sourceIndex]);
}

bool TextureSourceBinder::SetSourceInfo(const string& bindKey, const TextureSourceInfo& sourceInfo) noexcept
{
    auto it = m_bindingTable.find(bindKey);
    if (it == m_bindingTable.end()) return false;

    it->second = sourceInfo;
    return true;
}

void TextureSourceBinder::SerializeIO(JsonOperation& operation)
{
    operation.Process("BindingTable", m_bindingTable);
}

unique_ptr<TextureSourceBinder> CreateSourceBinder(const wstring& jsonFilename)
{
    auto sourceBinder = make_unique<TextureSourceBinder>();
    if (jsonFilename.empty()) return move(sourceBinder);

    return sourceBinder->Load(jsonFilename) ? move(sourceBinder) : nullptr;
}

