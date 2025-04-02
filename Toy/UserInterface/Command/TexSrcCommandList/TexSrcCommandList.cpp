#include "pch.h"
#include "TexSrcCommandList.h"
#include "TexSrcCommandRegistry.h"
#include "../../../Utility.h"

bool TexSrcCommandList::AddFontKey(TextureSourceBinder * sb, const wstring & bindingKey, const TextureFontInfo & fontInfo)
{
	return ApplyCommand<AddFontKeyCommand>(sb, bindingKey, fontInfo);
}