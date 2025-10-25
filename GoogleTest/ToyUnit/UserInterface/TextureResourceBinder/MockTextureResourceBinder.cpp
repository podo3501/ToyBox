#include "pch.h"
#include "MockTextureResourceBinder.h"

void MockTextureResourceBinder::RegisterMockFonts(const vector<pair<wstring, TextureFontInfo>>& infos) noexcept
{
	for (auto& [key, info] : infos)
		AddFontKey(key, info);
}

void MockTextureResourceBinder::RegisterMockTextures(const vector<pair<string, TextureSourceInfo>>& infos) noexcept
{
	for (auto& [key, info] : infos)
		AddTextureKey(key, info);
}