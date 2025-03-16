#pragma once

enum class ImagePart : int;
class JsonOperation;
struct TextureSourceInfo
{
	bool operator==(const TextureSourceInfo& o) const noexcept
	{
		wstring oFilename = o.filename;
		if (filename != oFilename) return false;
		if (sources != o.sources) return false;
		return true;
		//return (tie(filename, sources) == tie(o.filename, o.sources));
	}

	void SerializeIO(JsonOperation& operation);

	inline bool IsEmpty() const noexcept { return (filename.empty() && sources.empty()) ? true : false; }
	inline int GetSourceSize() const noexcept { return static_cast<int>(sources.size()); }
	inline Rectangle GetSource(int index) const noexcept { return (sources.size() > index) ? sources.at(index) : Rectangle{}; }
	inline void SetSource(int index, const Rectangle& area) noexcept { sources[index] = area; }

	wstring filename;
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
	bool ChangeBindingKey(const string& bindingKey, const TextureSourceInfo& sourceAreas) noexcept;
	vector<wstring> GetTextureFiles() const noexcept;
	string GetBindingKey(const TextureSourceInfo& sourceAreas) const noexcept;
	Rectangle GetArea(const string& key, int index) const noexcept;
	vector<TextureSourceInfo> GetAreas(const wstring& filename, ImagePart part) const noexcept;
	void SerializeIO(JsonOperation& operation);

private:
	wstring m_jsonFilename;
	unordered_map<string, TextureSourceInfo> m_bindingTable;
};

unique_ptr<TextureSourceBinder> CreateSourceBinder(const wstring& jsonFilename = L"");