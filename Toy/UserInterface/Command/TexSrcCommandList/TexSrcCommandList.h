#pragma once
#include "../Command.h"
#include "../CommandList.h"

struct TextureFontInfo;
class TextureSourceBinder;
class TexSrcCommandList : public CommandList<TexSrcCommand>
{
public:
	~TexSrcCommandList() = default;
	TexSrcCommandList() = default;

	bool AddFontKey(TextureSourceBinder* sb, const wstring& bindingKey, const TextureFontInfo& fontInfo);
};
