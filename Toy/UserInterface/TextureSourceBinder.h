#pragma once

enum class ImagePart : int;
class JsonOperation;
struct TextureSourceInfo
{
	~TextureSourceInfo();
	TextureSourceInfo();
	TextureSourceInfo(const wstring& _filename, ImagePart _imagePart, const vector<Rectangle>& _sources) noexcept;

	bool operator==(const TextureSourceInfo& o) const noexcept
	{
		return (tie(filename, sources) == tie(o.filename, o.sources));
	}

	void SerializeIO(JsonOperation& operation);
	inline Rectangle GetSource(int index) const noexcept { return (sources.size() > index) ? sources.at(index) : Rectangle{}; }

	wstring filename;
	ImagePart imagePart;
	vector<Rectangle> sources;
};

class TextureSourceBinder
{
public:
	~TextureSourceBinder();
	TextureSourceBinder();

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