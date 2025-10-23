#include "pch.h"
#include "TextureResourceBinderT.h"

namespace UserInterface
{
	TEST_F(TextureResourceBinderT, GetAreas)
	{
		vector<TextureSourceInfo> areas = GetAreas(m_resBinder.get(), L"MockTex.png", TextureSlice::One);
		EXPECT_FALSE(areas.empty());
	}

	TEST_F(TextureResourceBinderT, GetBindingKey)
	{
		TextureSourceInfo sourceInfo{ L"MockTex.png", TextureSlice::One, {} };
		EXPECT_EQ(m_resBinder->GetBindingKey(sourceInfo), "MockTexture");
	}

	TEST_F(TextureResourceBinderT, GetTextureKeys)
	{
		EXPECT_TRUE(m_resBinder->GetTextureKeys(TextureSlice::One).size());
	}

	TEST_F(TextureResourceBinderT, RenameFontKey)
	{
		m_resBinder->RenameFontKey(L"MockFont", L"NewKey");
		EXPECT_EQ(m_resBinder->GetTextureFontInfo(L"NewKey"), TextureFontInfo(L"MockFont.spritefont"));
	}

	TEST_F(TextureResourceBinderT, RenameTextureKey)
	{
		m_resBinder->RenameTextureKey("MockTexture", "NewKey");
		EXPECT_EQ(m_resBinder->GetTextureSourceInfo("NewKey"), TextureSourceInfo(L"MockTex.png", TextureSlice::One, {}));
	}

	TEST_F(TextureResourceBinderT, RemoveKeyByFilename)
	{
		m_resBinder->RemoveKeyByFilename(L"MockTex.png");
		m_resBinder->RemoveKeyByFilename(L"MockFont.spritefont");

		EXPECT_FALSE(m_resBinder->GetTextureFontInfo(L"MockFont"));
		EXPECT_FALSE(m_resBinder->GetTextureSourceInfo("MockTexture"));
	}

	TEST_F(TextureResourceBinderT, WriteRead)
	{
		wstring filename = L"Test/Data/RWSourceBinderTest.json";
		EXPECT_TRUE(m_resBinder->Save(filename));

		unique_ptr<TextureResourceBinder> read = make_unique<TextureResourceBinder>();
		EXPECT_TRUE(read->Load(filename));

		EXPECT_TRUE(*m_resBinder == *read);
	}
}