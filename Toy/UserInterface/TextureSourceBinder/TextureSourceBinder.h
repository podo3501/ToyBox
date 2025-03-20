#pragma once
#include "../Include/ITextureBinder.h"
#include "TextureSourceInfo.h" //TextureSourceInfo가 클래스로 바뀐다면 전방선언으로 바꾸는게 더 좋지 않을까 ?!?

enum class ImagePart : int;
struct ITextureLoad;
class JsonOperation;

class TextureSourceBinder : public ITextureBinder
{
public:
	~TextureSourceBinder();
	TextureSourceBinder();

	virtual bool LoadResources(ITextureLoad* load) override;

	bool Load(const wstring& jsonFilename);
	bool Save(const wstring& jsonFilename);
	inline const wstring& GetJsonFilename() const noexcept { return m_jsonFilename; }
	bool InsertBindingKey(const string& bindingKey, const TextureSourceInfo& sourceAreas) noexcept;
	bool ModifyBindingKey(const string& newKey, const string& preKey) noexcept;
	void RemoveBindingKey(const string& bindingKey) noexcept;
	void RemoveBindingKey(const wstring& filename) noexcept;
	
	Rectangle GetArea(const string& key, int index) const noexcept;

	vector<wstring> GetTextureFiles() const noexcept;
	string GetBindingKey(const TextureSourceInfo& sourceAreas) const noexcept;
	vector<Rectangle> GetArea(const wstring& filename, ImagePart imgPart, const XMINT2& position) const noexcept;
	vector<TextureSourceInfo> GetAreas(const wstring& filename, ImagePart part) const noexcept;
	pair<wstring, Rectangle> GetSourceInfo(const string& bindKey, size_t sourceIndex) const noexcept;
	bool SetSourceInfo(const string& bindKey, const TextureSourceInfo& sourceInfo) noexcept;
	void SerializeIO(JsonOperation& operation);

private:
	wstring m_jsonFilename;
	unordered_map<string, TextureSourceInfo> m_bindingTable;
};

unique_ptr<TextureSourceBinder> CreateSourceBinder(const wstring& jsonFilename = L"");