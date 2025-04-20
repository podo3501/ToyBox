#pragma once
#include "../Include/ITextureBinder.h"
#include "TextureSourceInfo.h"
#include "TextureFontInfo.h"
#include "TextureBinderHelper.h"

enum class TextureSlice : int;
struct ITextureLoad;
class JsonOperation;
class TextureResourceBinder : public ITextureBinder
{
public:
	~TextureResourceBinder();
	TextureResourceBinder();
	bool operator==(const TextureResourceBinder& o) const noexcept;

	virtual bool LoadResources(ITextureLoad* load) override;

	bool Load(const wstring& jsonFilename);
	bool Save(const wstring& jsonFilename);
	inline const wstring& GetJsonFilename() const noexcept { return m_jsonFilename; }
	bool AddFontKey(const wstring& bindingKey, const TextureFontInfo& fontInfo) noexcept;
	bool AddTextureKey(const string& bindingKey, const TextureSourceInfo& sourceAreas) noexcept;
	bool ModifyTextureSourceInfo(const string& bindKey, const TextureSourceInfo& sourceInfo) noexcept;
	void RemoveFontKey(const wstring& bindingKey) noexcept;
	void RemoveTextureKey(const string& bindingKey) noexcept;
	bool RemoveKeyByFilename(const wstring& filename) noexcept;

	bool RenameFontKey(const wstring& preKey, const wstring& newKey) noexcept;
	bool RenameTextureKey(const string& preKey, const string& newKey) noexcept;
	
	optionalRef<TextureSourceInfo> GetTextureSourceInfo(const string& key) const noexcept;
	optionalRef<TextureFontInfo> GetTextureFontInfo(const wstring& key) const noexcept;

	string GetBindingKey(const TextureSourceInfo& sourceAreas) const noexcept;
	vector<string> GetTextureKeys(TextureSlice texSlice) const noexcept;
	vector<string> GetTextureKeys(const wstring& filename) const noexcept;
	vector<string> GetTextureAllKeys() const noexcept;
	wstring GetFontKey(const wstring& fontFilename) const noexcept;
	vector<wstring> GetTextureFiles() const noexcept;

	vector<TextureSourceInfo> GetTotalAreas(const wstring& filename) const noexcept;
	void SerializeIO(JsonOperation& operation);

private:
	wstring m_jsonFilename;
	unordered_map<wstring, TextureFontInfo> m_bindingFontTable;
	unordered_map<string, TextureSourceInfo> m_bindingTexTable;
};

unique_ptr<TextureResourceBinder> CreateSourceBinder(const wstring& jsonFilename = L"");