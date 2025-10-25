#pragma once
#include "Toy/UserInterface/TextureResourceBinder/TextureResourceBinder.h"

class MockTextureResourceBinder : public TextureResourceBinder
{
public:
	void RegisterMockFonts(const vector<pair<wstring, TextureFontInfo>>& infos) noexcept;
	void RegisterMockTextures(const vector<pair<string, TextureSourceInfo>>& infos) noexcept;
};
