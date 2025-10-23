#pragma once
#include "Mocks/MockRenderer.h"
#include "Toy/UserInterface/TextureResourceBinder/TextureLoadBinder.h"

class TextureLoadBinderT : public ::testing::Test
{
protected:
	virtual void SetUp() override;
	virtual void TearDown() override {};

	unique_ptr<MockRenderer> m_mockRenderer{ nullptr };
	unique_ptr<TextureLoadBinder> m_loadBinder{ nullptr };
};

///////////////////////////////////////////////////////////////

void TextureLoadBinderT::SetUp()
{
	m_mockRenderer = make_unique<MockRenderer>();
	m_loadBinder = make_unique<TextureLoadBinder>();

	m_mockRenderer->RegisterMockTextureInfo(L"MockTexture.png", { 512, 512 });
}