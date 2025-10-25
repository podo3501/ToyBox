#pragma once
#include "MockTextureResourceBinder.h"
#include "Shared/Framework/EnvironmentLocator.h"
#include "Toy/UserInterface/TextureResourceBinder/TextureResourceBinder.h"
#include "Toy/UserInterface/UIComponent/UIType.h"

class TextureResourceBinderT : public ::testing::Test
{
protected:
	virtual void SetUp() override;
	virtual void TearDown() override {};

	unique_ptr<Environment> m_environment;
	unique_ptr<MockTextureResourceBinder> m_resBinder;
};

///////////////////////////////////////////////////////////////

void TextureResourceBinderT::SetUp()
{
	m_environment = InitializeEnvironment(L"../Resources/", { 800.f, 600.f });
	m_resBinder = make_unique<MockTextureResourceBinder>();

	m_resBinder->RegisterMockFonts({
		{L"MockFont", TextureFontInfo{L"MockFont.spritefont"}}
		});
	m_resBinder->RegisterMockTextures({
		{"MockTexture", {L"MockTex.png", TextureSlice::One, {}}}
		});
}