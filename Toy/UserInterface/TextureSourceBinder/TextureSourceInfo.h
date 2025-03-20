#pragma once
enum class ImagePart : int;
struct ITextureLoad;
class JsonOperation;

struct TextureSourceInfo
{
	~TextureSourceInfo();
	TextureSourceInfo() noexcept;
	TextureSourceInfo(const TextureSourceInfo& other) noexcept; //stl container를 쓴다면 기본 복사 생성자가 생성되어서 원하지 않는 값 복사가 일어날 수 있다.
	TextureSourceInfo(const wstring& _filename, ImagePart _imagePart, const vector<Rectangle>& _sources) noexcept;
	bool operator==(const TextureSourceInfo& o) const noexcept;

	bool LoadResource(ITextureLoad* load);
	void SerializeIO(JsonOperation& operation);
	inline Rectangle GetSource(int index) const noexcept { return (sources.size() > index) ? sources.at(index) : Rectangle{}; }

	wstring filename;
	ImagePart imagePart;
	vector<Rectangle> sources;

private:
	void Release() noexcept;

	ITextureLoad* m_texLoader;
	optional<size_t> m_index;
};