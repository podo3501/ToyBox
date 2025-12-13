#include "pch.h"
#include "MockTextureResourceBinder.h"

void AddMockFontKeys(TextureResourceBinder* binder, const vector<pair<wstring, TextureFontInfo>>& infos) noexcept
{
	for (auto& [key, info] : infos)
		binder->AddFontKey(key, info);
}

void AddMockTextureKeys(TextureResourceBinder* binder, const vector<pair<string, TextureSourceInfo>>& infos) noexcept
{
	for (auto& [key, info] : infos)
		binder->AddTextureKey(key, info);
}

void MockTextureResourceBinder::AddMockFontKeys(const vector<pair<wstring, TextureFontInfo>>& infos) noexcept
{
	::AddMockFontKeys(this, infos);
}

void MockTextureResourceBinder::AddMockTextureKeys(const vector<pair<string, TextureSourceInfo>>& infos) noexcept
{
	::AddMockTextureKeys(this, infos);
}