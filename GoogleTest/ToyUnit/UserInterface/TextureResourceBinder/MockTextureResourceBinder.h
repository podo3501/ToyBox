#pragma once
#include "Toy/UserInterface/TextureResourceBinder/TextureResourceBinder.h"

class MockTextureResourceBinder : public TextureResourceBinder
{
public:
	void AddMockFontKeys(const vector<pair<wstring, TextureFontInfo>>& infos) noexcept;
	void AddMockTextureKeys(const vector<pair<string, TextureSourceInfo>>& infos) noexcept;
};

void AddMockFontKeys(TextureResourceBinder* binder, const vector<pair<wstring, TextureFontInfo>>& infos) noexcept;
void AddMockTextureKeys(TextureResourceBinder* binder, const vector<pair<string, TextureSourceInfo>>& infos) noexcept;
