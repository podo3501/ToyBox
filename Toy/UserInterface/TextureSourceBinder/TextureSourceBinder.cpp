#include "pch.h"
#include "TextureSourceBinder.h"
#include "../JsonHelper.h"
#include "../../Utility.h"
#include "../UIUtility.h"

TextureSourceBinder::~TextureSourceBinder() = default;
TextureSourceBinder::TextureSourceBinder()
{}

bool TextureSourceBinder::operator==(const TextureSourceBinder& o) const noexcept
{
    ReturnIfFalse(tie(m_jsonFilename, m_bindingFontTable, m_bindingTexTable) ==
        tie(o.m_jsonFilename, o.m_bindingFontTable, o.m_bindingTexTable));

    return true;
}

bool TextureSourceBinder::LoadResources(ITextureLoad* load)
{
    bool fontResult = ranges::all_of(m_bindingFontTable, [load](auto& pair) {
        return pair.second.LoadResource(load);
        });

    bool texResult = ranges::all_of(m_bindingTexTable, [load](auto& pair) {
        return pair.second.LoadResource(load);
        });

    return (fontResult && texResult);
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

template <typename MapType, typename KeyType, typename Valuetype>
static bool InsertBindingImpl(MapType& bindingTable, const KeyType& bindingKey, const Valuetype& value) noexcept
{
    if (bindingKey.empty()) return true;

    if (auto it = bindingTable.find(bindingKey); it != bindingTable.end())
    {
        if (it->second != value) return false; // 같은 이름이 있지만 값이 다를 경우
        return true; // 같은 이름이고 값도 같으면 처리 안함
    }

    bindingTable.emplace(bindingKey, value); // 새로운 항목 삽입
    return true;
}

bool TextureSourceBinder::InsertBindingKey(const wstring& bindingKey, const TextureFontInfo& fontInfo) noexcept
{
    return InsertBindingImpl(m_bindingFontTable, bindingKey, fontInfo);
}

bool TextureSourceBinder::InsertBindingKey(const string& bindingKey, const TextureSourceInfo& sourceAreas) noexcept
{
    return InsertBindingImpl(m_bindingTexTable, bindingKey, sourceAreas);
}

template <typename MapType, typename KeyType>
static bool ModifyBindingImpl(MapType& bindingTable, const KeyType& preKey, const KeyType& newKey) noexcept
{
    if (auto it = bindingTable.find(newKey); it != bindingTable.end()) return false;
    if (auto it = bindingTable.find(preKey); it != bindingTable.end())
    {
        auto value = move(it->second);
        bindingTable.erase(it);
        bindingTable.emplace(newKey, move(value));
    }
    return true;
}

bool TextureSourceBinder::ModifyBindingKey(const string& preKey, const string& newKey) noexcept
{
    return ModifyBindingImpl(m_bindingTexTable, preKey, newKey);
}

bool TextureSourceBinder::ModifyBindingKey(const wstring& preKey, const wstring& newKey) noexcept
{
    return ModifyBindingImpl(m_bindingFontTable, preKey, newKey);
}

void TextureSourceBinder::RemoveBindingKey(const string& bindingKey) noexcept
{
    if (auto it = m_bindingTexTable.find(bindingKey); it != m_bindingTexTable.end())
        m_bindingTexTable.erase(it);
}

bool TextureSourceBinder::RemoveBindingKey(const wstring& filename) noexcept
{
    erase_if(m_bindingTexTable, [&filename](const auto& pair) {
        return pair.second.filename == filename;
        });

    erase_if(m_bindingFontTable, [&filename](const auto& pair) {
        return pair.second.filename == filename;
        });

    return true;
}

vector<wstring> TextureSourceBinder::GetTextureFiles() const noexcept
{
    unordered_set<wstring> seenFilenames;

    for (const auto& entry : m_bindingTexTable) //중복 방지를 위해서 set에 넣음
        seenFilenames.insert(entry.second.filename);

    return vector<wstring>(seenFilenames.begin(), seenFilenames.end());
}

string TextureSourceBinder::GetBindingKey(const TextureSourceInfo& sourceAreas) const noexcept
{
    auto it = ranges::find_if(m_bindingTexTable, [&](const auto& pair) { return pair.second == sourceAreas; });
    return (it != m_bindingTexTable.end()) ? it->first : "";
}

wstring TextureSourceBinder::GetBindingKey(const wstring& fontFilename) const noexcept
{
    auto it = ranges::find_if(m_bindingFontTable, [&](const auto& pair) { return pair.second.filename == fontFilename; });
    return (it != m_bindingFontTable.end()) ? it->first : L"";
}

Rectangle TextureSourceBinder::GetArea(const string& key, int index) const noexcept
{
    auto it = m_bindingTexTable.find(key);
    if (it == m_bindingTexTable.end()) return {};

    return it->second.GetSource(index);
}

vector<Rectangle> TextureSourceBinder::GetArea(const wstring& filename, ImagePart imgPart, const XMINT2& position) const noexcept
{
    for (const auto& entry : m_bindingTexTable) {
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
    for (const auto& entry : m_bindingTexTable) {
        const TextureSourceInfo& sourceInfo = entry.second;
        if (sourceInfo.filename == filename && sourceInfo.imagePart == part)
            filteredTextures.push_back(sourceInfo);
    }
    return filteredTextures;
}

pair<wstring, Rectangle> TextureSourceBinder::GetSourceInfo(const string& bindKey, size_t sourceIndex) const noexcept
{
    auto it = m_bindingTexTable.find(bindKey);
    if (it == m_bindingTexTable.end() || it->second.sources.size() <= sourceIndex) return {};

    return make_pair(it->second.filename, it->second.sources[sourceIndex]);
}

bool TextureSourceBinder::SetSourceInfo(const string& bindKey, const TextureSourceInfo& sourceInfo) noexcept
{
    auto it = m_bindingTexTable.find(bindKey);
    if (it == m_bindingTexTable.end()) return false;

    it->second = sourceInfo;
    return true;
}

void TextureSourceBinder::SerializeIO(JsonOperation& operation)
{
    operation.Process("BindingFontTable", m_bindingFontTable);
    operation.Process("BindingTexTable", m_bindingTexTable);
}

unique_ptr<TextureSourceBinder> CreateSourceBinder(const wstring& jsonFilename)
{
    auto sourceBinder = make_unique<TextureSourceBinder>();
    if (jsonFilename.empty()) return move(sourceBinder);

    return sourceBinder->Load(jsonFilename) ? move(sourceBinder) : nullptr;
}

