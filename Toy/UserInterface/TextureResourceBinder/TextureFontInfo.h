#pragma once
#include "TextureInfo.h"

struct ITextureLoad;
class JsonOperation;
struct TextureFontInfo : public TextureInfo
{
	~TextureFontInfo();
	TextureFontInfo() noexcept;
	TextureFontInfo(const TextureFontInfo& other) noexcept;
	explicit TextureFontInfo(const wstring& _filename) noexcept;
	bool operator==(const TextureFontInfo& o) const noexcept;

	bool LoadResource(ITextureLoad* load);
	void SerializeIO(JsonOperation& operation);

	using TextureInfo::GetIndex;

	wstring filename;
};