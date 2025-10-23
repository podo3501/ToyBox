#pragma once
#include "Shared/Framework/Environment.h"
#include "Toy/UserInterface/TextureResourceBinder/TextureResourceBinder.h"
#include "Toy/UserInterface/UIComponent/UIType.h"

class TextureResourceBinderT : public ::testing::Test
{
protected:
	virtual void SetUp() override;
	virtual void TearDown() override {};

	unique_ptr<TextureResourceBinder> m_resBinder{ nullptr };

private:
	void AddFonts() noexcept;
	void AddTextures() noexcept;
	void AddMockFont(wstring&& key, wstring&& filename) noexcept;
	void AddMockTexture(string&& key, wstring&& filename, TextureSlice slice, vector<Rectangle>&& areas) noexcept;
};

///////////////////////////////////////////////////////////////

void TextureResourceBinderT::SetUp()
{
	InitializeEnvironment(L"../Resources/", { 800.f, 600.f });
	m_resBinder = CreateTextureResourceBinder();

	AddFonts();
	AddTextures();
}

void TextureResourceBinderT::AddFonts() noexcept
{
	AddMockFont(L"MockFont", L"MockFont.spritefont");
}

void TextureResourceBinderT::AddTextures() noexcept
{
	AddMockTexture("MockTexture", L"MockTex.png", TextureSlice::One, {});
}

void TextureResourceBinderT::AddMockFont(wstring&& key, wstring&& filename) noexcept
{
	TextureFontInfo info{ move(filename) };
	m_resBinder->AddFontKey(move(key), info);
}

void TextureResourceBinderT::AddMockTexture(string&& key, wstring&& filename, 
	TextureSlice slice, vector<Rectangle>&& areas) noexcept
{
	TextureSourceInfo info{ move(filename), slice, move(areas) };
	m_resBinder->AddTextureKey(move(key), info);
}