#pragma once
#include "../Include/ITextureBinder.h"
#include "TextureSourceInfo.h" //TextureSourceInfo가 클래스로 바뀐다면 전방선언으로 바꾸는게 더 좋지 않을까 ?!?
#include "TextureFontInfo.h"
#include "TextureBinderHelper.h"

enum class ImagePart : int;
struct ITextureLoad;
class JsonOperation;
class TextureSourceBinder : public ITextureBinder
{
public:
	~TextureSourceBinder();
	TextureSourceBinder();
	bool operator==(const TextureSourceBinder& o) const noexcept;

	virtual bool LoadResources(ITextureLoad* load) override;

	bool Load(const wstring& jsonFilename);
	bool Save(const wstring& jsonFilename);
	inline const wstring& GetJsonFilename() const noexcept { return m_jsonFilename; }
	bool InsertFontKey(const wstring& bindingKey, const TextureFontInfo& fontInfo) noexcept;
	bool InsertTextureKey(const string& bindingKey, const TextureSourceInfo& sourceAreas) noexcept; //?!? 데이터를 넣는것이니까 이름을 딴걸로
	bool ModifyFontKey(const wstring& preKey, const wstring& newKey) noexcept; //?!? 키를 바꾸는 거니 modify말고 딴걸로
	bool ModifyTextureKey(const string& preKey, const string& newKey) noexcept;
	void RemoveTextureKey(const string& bindingKey) noexcept;
	bool RemoveKeyByFilename(const wstring& filename) noexcept;
	
	optionalRef<TextureSourceInfo> GetTextureSourceInfo(const string& key) const noexcept;
	optionalRef<TextureFontInfo> GetFontSourceInfo(const wstring& key) const noexcept;

	string GetBindingKey(const TextureSourceInfo& sourceAreas) const noexcept;
	wstring GetBindingKey(const wstring& fontFilename) const noexcept;
	vector<wstring> GetTextureFiles() const noexcept;

	vector<Rectangle> GetAreas(const wstring& filename, ImagePart imgPart, const XMINT2& position) const noexcept;
	vector<TextureSourceInfo> GetAreas(const wstring& filename, ImagePart part) const noexcept;
	bool SetSourceInfo(const string& bindKey, const TextureSourceInfo& sourceInfo) noexcept;
	void SerializeIO(JsonOperation& operation);

private:
	wstring m_jsonFilename;
	unordered_map<wstring, TextureFontInfo> m_bindingFontTable;
	unordered_map<string, TextureSourceInfo> m_bindingTexTable;
};

unique_ptr<TextureSourceBinder> CreateSourceBinder(const wstring& jsonFilename = L"");