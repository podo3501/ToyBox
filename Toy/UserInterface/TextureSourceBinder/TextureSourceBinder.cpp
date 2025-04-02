#include "pch.h"
#include "TextureSourceBinder.h"
#include "../JsonOperation/JsonHelper.h"
#include "../../Utility.h"
#include "../UIComponent/UIUtility.h"

TextureSourceBinder::~TextureSourceBinder() = default;
TextureSourceBinder::TextureSourceBinder()
{}

bool TextureSourceBinder::operator==(const TextureSourceBinder& o) const noexcept
{
    ReturnIfFalse(tie(m_bindingFontTable, m_bindingTexTable) == tie(o.m_bindingFontTable, o.m_bindingTexTable));

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
static bool AddBindingImpl(MapType& bindingTable, const KeyType& bindingKey, const Valuetype& value) noexcept
{
    if (bindingKey.empty()) return true;

    if (auto it = bindingTable.find(bindingKey); it != bindingTable.end())
    {
        if (it->second != value) return false; // ���� �̸��� ������ ���� �ٸ� ���
        return true; // ���� �̸��̰� ���� ������ ó�� ����
    }

    bindingTable.emplace(bindingKey, value); // ���ο� �׸� ����
    return true;
}

bool TextureSourceBinder::AddFontKey(const wstring& bindingKey, const TextureFontInfo& fontInfo) noexcept
{
    return AddBindingImpl(m_bindingFontTable, bindingKey, fontInfo);
}

bool TextureSourceBinder::AddTextureKey(const string& bindingKey, const TextureSourceInfo& sourceAreas) noexcept
{
    return AddBindingImpl(m_bindingTexTable, bindingKey, sourceAreas);
}

void TextureSourceBinder::RemoveFontKey(const wstring& bindingKey) noexcept
{
    if (auto it = m_bindingFontTable.find(bindingKey); it != m_bindingFontTable.end())
        m_bindingFontTable.erase(it);
}

void TextureSourceBinder::RemoveTextureKey(const string& bindingKey) noexcept
{
    if (auto it = m_bindingTexTable.find(bindingKey); it != m_bindingTexTable.end())
        m_bindingTexTable.erase(it);
}

bool TextureSourceBinder::RemoveKeyByFilename(const wstring& filename) noexcept
{
    erase_if(m_bindingTexTable, [&filename](const auto& pair) {
        return pair.second.filename == filename;
        });

    erase_if(m_bindingFontTable, [&filename](const auto& pair) {
        return pair.second.filename == filename;
        });

    return true;
}

template <typename MapType, typename KeyType>
static bool RenameBindingImpl(MapType& bindingTable, const KeyType& preKey, const KeyType& newKey) noexcept
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

bool TextureSourceBinder::RenameFontKey(const wstring& preKey, const wstring& newKey) noexcept
{
    return RenameBindingImpl(m_bindingFontTable, preKey, newKey);
}

bool TextureSourceBinder::RenameTextureKey(const string& preKey, const string& newKey) noexcept
{
    return RenameBindingImpl(m_bindingTexTable, preKey, newKey);
}

vector<wstring> TextureSourceBinder::GetTextureFiles() const noexcept
{
    unordered_set<wstring> seenFilenames;

    for (const auto& entry : m_bindingTexTable) //�ߺ� ������ ���ؼ� set�� ����
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

vector<string> TextureSourceBinder::GetTextureKeys(ImagePart imgPart) const noexcept
{
    vector<string> imgPartKeys;
    for (const auto& pair : m_bindingTexTable)
    {
        const auto& srcInfo = pair.second;
        if (srcInfo.imagePart != imgPart) continue;
        
        imgPartKeys.emplace_back(pair.first);
    }

    return imgPartKeys;
}

optionalRef<TextureSourceInfo> TextureSourceBinder::GetTextureSourceInfo(const string& key) const noexcept
{
    auto it = m_bindingTexTable.find(key);
    if (it == m_bindingTexTable.end()) return nullopt;

    return cref(it->second);
}

optionalRef<TextureFontInfo> TextureSourceBinder::GetFontSourceInfo(const wstring& key) const noexcept
{
    auto it = m_bindingFontTable.find(key);
    if (it == m_bindingFontTable.end()) return nullopt;

    return cref(it->second);
}

//?!? �Ļ��� �Լ����� ��� �Լ��� ������ ���� GetSourceInfo�� ����ؼ� ��ƿ��Ƽ �Լ��� ����� �������.
vector<Rectangle> TextureSourceBinder::GetAreas(const wstring& filename, ImagePart imgPart, const XMINT2& position) const noexcept
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

/////////////////////////////////////////////////////////////////////////

unique_ptr<TextureSourceBinder> CreateSourceBinder(const wstring& jsonFilename)
{
    auto sourceBinder = make_unique<TextureSourceBinder>();
    if (jsonFilename.empty()) return move(sourceBinder);

    return sourceBinder->Load(jsonFilename) ? move(sourceBinder) : nullptr;
}
