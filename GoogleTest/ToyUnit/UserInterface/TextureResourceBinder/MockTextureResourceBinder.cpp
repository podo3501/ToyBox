#include "pch.h"
#include "MockTextureResourceBinder.h"

void MockTextureResourceBinder::AddMockFontKeys(const vector<pair<wstring, TextureFontInfo>>& infos) noexcept
{
	for (auto& [key, info] : infos)
		AddFontKey(key, info);
}

void MockTextureResourceBinder::AddMockTextureKeys(const vector<pair<string, TextureSourceInfo>>& infos) noexcept
{
	for (auto& [key, info] : infos)
		AddTextureKey(key, info);
}