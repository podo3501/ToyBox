#include "pch.h"
#include "TextureResourceBinder.h"
#include "../JsonOperation/JsonHelper.h"
#include "../../Utility.h"
#include "../UIComponent/UIUtility.h"

TextureResourceBinder::~TextureResourceBinder() = default;
TextureResourceBinder::TextureResourceBinder()
{}

bool TextureResourceBinder::operator==(const TextureResourceBinder& o) const noexcept
{
    ReturnIfFalse(tie(m_bindingFontTable, m_bindingTexTable) == tie(o.m_bindingFontTable, o.m_bindingTexTable));

    return true;
}

bool TextureResourceBinder::LoadResources(ITextureLoad* load)
{
    bool fontResult = ranges::all_of(m_bindingFontTable, [load](auto& pair) {
        return pair.second.LoadResource(load);
        });

    bool texResult = ranges::all_of(m_bindingTexTable, [load](auto& pair) {
        return pair.second.LoadResource(load);
        });

    return (fontResult && texResult);
}

bool TextureResourceBinder::Load(const wstring& jsonFilename)
{
    ReturnIfFalse(JsonFile::Read(jsonFilename, *this));
    m_jsonFilename = jsonFilename;

    return true;
}

bool TextureResourceBinder::Save(const wstring& jsonFilename)
{
    ReturnIfFalse(JsonFile::Write(*this, jsonFilename));
    m_jsonFilename = jsonFilename;

    return true;
}

static bool AddBindingImpl(auto& bindingTable, const auto& bindingKey, const auto& value) noexcept
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

bool TextureResourceBinder::AddFontKey(const wstring& bindingKey, const TextureFontInfo& fontInfo) noexcept
{
    return AddBindingImpl(m_bindingFontTable, bindingKey, fontInfo);
}

bool TextureResourceBinder::AddTextureKey(const string& bindingKey, const TextureSourceInfo& sourceAreas) noexcept
{
    return AddBindingImpl(m_bindingTexTable, bindingKey, sourceAreas);
}

bool TextureResourceBinder::ModifyTextureSourceInfo(const string& bindKey, const TextureSourceInfo& sourceInfo) noexcept
{
    auto it = m_bindingTexTable.find(bindKey);
    if (it == m_bindingTexTable.end()) return false;

    it->second = sourceInfo;
    return true;
}

void TextureResourceBinder::RemoveFontKey(const wstring& bindingKey) noexcept
{
    if (auto it = m_bindingFontTable.find(bindingKey); it != m_bindingFontTable.end())
        m_bindingFontTable.erase(it);
}

void TextureResourceBinder::RemoveTextureKey(const string& bindingKey) noexcept
{
    if (auto it = m_bindingTexTable.find(bindingKey); it != m_bindingTexTable.end())
        m_bindingTexTable.erase(it);
}

bool TextureResourceBinder::RemoveKeyByFilename(const wstring& filename) noexcept
{
    erase_if(m_bindingTexTable, [&filename](const auto& pair) {
        return pair.second.filename == filename;
        });

    erase_if(m_bindingFontTable, [&filename](const auto& pair) {
        return pair.second.filename == filename;
        });

    return true;
}

static bool RenameBindingImpl(auto& bindingTable, const auto& preKey, const auto& newKey) noexcept
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

bool TextureResourceBinder::RenameFontKey(const wstring& preKey, const wstring& newKey) noexcept
{
    return RenameBindingImpl(m_bindingFontTable, preKey, newKey);
}

bool TextureResourceBinder::RenameTextureKey(const string& preKey, const string& newKey) noexcept
{
    return RenameBindingImpl(m_bindingTexTable, preKey, newKey);
}

vector<wstring> TextureResourceBinder::GetTextureFiles() const noexcept
{
    unordered_set<wstring> seenFilenames;

    for (const auto& entry : m_bindingTexTable) //중복 방지를 위해서 set에 넣음
        seenFilenames.insert(entry.second.filename);

    return vector<wstring>(seenFilenames.begin(), seenFilenames.end());
}

string TextureResourceBinder::GetBindingKey(const TextureSourceInfo& sourceAreas) const noexcept
{
    auto it = ranges::find_if(m_bindingTexTable, [&](const auto& pair) { return pair.second == sourceAreas; });
    return (it != m_bindingTexTable.end()) ? it->first : "";
}

wstring TextureResourceBinder::GetFontKey(const wstring& fontFilename) const noexcept
{
    auto it = ranges::find_if(m_bindingFontTable, [&](const auto& pair) { return pair.second.filename == fontFilename; });
    return (it != m_bindingFontTable.end()) ? it->first : L"";
}

static vector<string> FilterTextureKeys(const auto& bindTexTable, auto predicate) noexcept
{
    vector<string> keys;
    keys.reserve(bindTexTable.size());

    for (const auto& pair : bindTexTable)
        if (predicate(pair.second))
            keys.emplace_back(pair.first);
    
    return keys;
}

vector<string> TextureResourceBinder::GetTextureKeys(ImagePart imgPart) const noexcept
{
    return FilterTextureKeys(m_bindingTexTable, [imgPart](const TextureSourceInfo& info) { return info.imagePart == imgPart; });
}

vector<string> TextureResourceBinder::GetTextureKeys(const wstring& filename) const noexcept
{
    return FilterTextureKeys(m_bindingTexTable, [&filename](const TextureSourceInfo& info) { return info.filename == filename; });
}

optionalRef<TextureSourceInfo> TextureResourceBinder::GetTextureSourceInfo(const string& key) const noexcept
{
    auto it = m_bindingTexTable.find(key);
    if (it == m_bindingTexTable.end()) return nullopt;

    return cref(it->second);
}

optionalRef<TextureFontInfo> TextureResourceBinder::GetTextureFontInfo(const wstring& key) const noexcept
{
    auto it = m_bindingFontTable.find(key);
    if (it == m_bindingFontTable.end()) return nullopt;

    return cref(it->second);
}

vector<TextureSourceInfo> TextureResourceBinder::GetTotalAreas(const wstring& filename) const noexcept
{
    vector<TextureSourceInfo> filteredTextures;
    for (const auto& entry : m_bindingTexTable) {
        const TextureSourceInfo& sourceInfo = entry.second;
        if (sourceInfo.filename == filename) filteredTextures.push_back(sourceInfo);
    }

    return filteredTextures;
}

void TextureResourceBinder::SerializeIO(JsonOperation& operation)
{
    operation.Process("BindingFontTable", m_bindingFontTable);
    operation.Process("BindingTexTable", m_bindingTexTable);
}

/////////////////////////////////////////////////////////////////////////

unique_ptr<TextureResourceBinder> CreateSourceBinder(const wstring& jsonFilename)
{
    auto resBinder = make_unique<TextureResourceBinder>();
    if (jsonFilename.empty()) return move(resBinder);

    return resBinder->Load(jsonFilename) ? move(resBinder) : nullptr;
}
