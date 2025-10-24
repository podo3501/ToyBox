#include "pch.h"
#include "TextureLoadBinderT.h"

namespace UserInterface::TextureResourceBinderT
{
	TEST_F(TextureLoadBinderT, LoadTexture)
	{
		wstring filename = L"MockTexture.png";
		EXPECT_TRUE(m_loadBinder->LoadTexture(m_mockRenderer.get(), filename));
	}
}