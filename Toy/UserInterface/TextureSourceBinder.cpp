#include "pch.h"
#include "TextureSourceBinder.h"
#include "JsonOperation.h"
#include "JsonHelper.h"
#include "UIType.h"
#include "../Utility.h"

void TextureSourceInfo::SerializeIO(JsonOperation& operation)
{
    operation.Process("Filename", filename);
    operation.Process("Sources", sources);
}

/////////////////////////////////////////////////////////////////

TextureSourceBinder::~TextureSourceBinder() = default;
TextureSourceBinder::TextureSourceBinder()
{}

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

bool TextureSourceBinder::ChangeBindingKey(const string& bindingKey, const TextureSourceInfo& sourceAreas) noexcept
{
    if (auto it = m_bindingTable.find(bindingKey); it != m_bindingTable.end())
    {
        if (it->second != sourceAreas) return false; //같은 이름이 있다.
        return true; 
    }

    erase_if(m_bindingTable, [&](const auto& pair) { //일단 지운다.
        return pair.second == sourceAreas;
        });

    if (bindingKey.empty()) return true; //키가 없다면 삭제한다고 간주한다.
    m_bindingTable.emplace(bindingKey, sourceAreas);
    return true;
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

vector<TextureSourceInfo> TextureSourceBinder::GetAreas(const wstring& filename, ImagePart part) const noexcept
{
    vector<TextureSourceInfo> filteredTextures;
    for (const auto& entry : m_bindingTable) {
        const TextureSourceInfo& sourceInfo = entry.second;
        if (sourceInfo.filename == filename && sourceInfo.sources.size() == static_cast<int>(part)) 
            filteredTextures.push_back(sourceInfo);
    }
    return filteredTextures;
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

