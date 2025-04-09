#pragma once
#include "TextureInfo.h"

enum class TextureSlice : int;
struct ITextureLoad;
class JsonOperation;
struct TextureSourceInfo : public TextureInfo
{
	~TextureSourceInfo();
	TextureSourceInfo() noexcept;
	TextureSourceInfo(const TextureSourceInfo&) = default; //stl container�� ���ٸ� �⺻ ���� �����ڰ� �����Ǿ ������ �ʴ� �� ���簡 �Ͼ �� �ִ�.
	TextureSourceInfo(TextureSourceInfo&&) = default;//emplace�Ҷ� ��������ڸ� ������ �̵��� �ȵ�.
	explicit TextureSourceInfo(const wstring& _filename) noexcept;
	TextureSourceInfo(const wstring& _filename, TextureSlice _texSlice, const vector<Rectangle>& _sources) noexcept;
	TextureSourceInfo& operator=(const TextureSourceInfo&) = default;
	bool operator==(const TextureSourceInfo& o) const noexcept;

	bool LoadResource(ITextureLoad* load);
	void SerializeIO(JsonOperation& operation);
	inline Rectangle GetSource(size_t index) const noexcept { return (sources.size() > index) ? sources.at(index) : Rectangle{}; }

	using TextureInfo::GetIndex;
	using TextureInfo::GetGfxOffset;

	wstring filename;
	TextureSlice texSlice;
	vector<Rectangle> sources;
};