#include "pch.h"
#include "TextureResourceBinderT.h"

namespace UserInterface::TextureResourceBinderT
{
	TEST_F(TextureResourceBinderT, ComputeSliceRects)
	{
		TextureSourceInfo info = m_resBinder->GetTextureSourceInfo("ThreeH48")->get();

		SourceDivider divider = GetSourceDivider(m_resBinder.get(), "ThreeH48");
		divider.list = { 20, 28 }; //�� ��Ʈ�� �����µ� 2���� x�� �������� �����Ѵ�. 22, 26���� 20, 28�� ����
		info.sources = ComputeSliceRects(info.texSlice, divider);

		EXPECT_EQ(info.sources[0], Rectangle(0, 0, 20, 48));
		EXPECT_EQ(info.sources[1], Rectangle(20, 0, 8, 48));
		EXPECT_EQ(info.sources[2], Rectangle(28, 0, 20, 48));
	}

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
		auto info = m_resBinder->GetTextureFontInfo(L"NewKey");

		EXPECT_TRUE(info);
		EXPECT_EQ(info->get().filename, L"MockFont.spritefont");
	}

	TEST_F(TextureResourceBinderT, RenameTextureKey)
	{
		m_resBinder->RenameTextureKey("MockTexture", "NewKey");
		auto info = m_resBinder->GetTextureSourceInfo("NewKey");

		EXPECT_TRUE(info);
		EXPECT_EQ(info->get().filename, L"MockTex.png");
	}

	TEST_F(TextureResourceBinderT, RemoveKeyByFilename)
	{
		m_resBinder->RemoveKeyByFilename(L"MockTex.png");
		m_resBinder->RemoveKeyByFilename(L"MockFont.spritefont");

		EXPECT_FALSE(m_resBinder->GetTextureSourceInfo("MockTexture"));
		EXPECT_FALSE(m_resBinder->GetTextureFontInfo(L"MockFont"));
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