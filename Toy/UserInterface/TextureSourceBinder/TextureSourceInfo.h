#pragma once
#include "TextureInfo.h"

enum class ImagePart : int;
struct ITextureLoad;
class JsonOperation;
struct TextureSourceInfo : public TextureInfo
{
	~TextureSourceInfo();
	TextureSourceInfo() noexcept;
	TextureSourceInfo(const TextureSourceInfo& other) noexcept; //stl container�� ���ٸ� �⺻ ���� �����ڰ� �����Ǿ ������ �ʴ� �� ���簡 �Ͼ �� �ִ�.
	TextureSourceInfo(const wstring& _filename, ImagePart _imagePart, const vector<Rectangle>& _sources) noexcept;
	bool operator==(const TextureSourceInfo& o) const noexcept;

	bool LoadResource(ITextureLoad* load);
	void SerializeIO(JsonOperation& operation);
	inline Rectangle GetSource(size_t index) const noexcept { return (sources.size() > index) ? sources.at(index) : Rectangle{}; }

	using TextureInfo::GetIndex;

	wstring filename;
	ImagePart imagePart;
	vector<Rectangle> sources;
};