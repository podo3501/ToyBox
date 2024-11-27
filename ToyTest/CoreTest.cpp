#include "pch.h"
#include "CoreTestFixture.h"
#include "../Core/TextureIndexing.h"
#include "../Core/DeviceResources.h"
#include "../Core/Utility.h"

namespace BasicCore
{
	bool LoadTexture(ILoadData* load)
	{
		wstring m_filename{ L"Resources/UI/Blue/button_square_header_large_square_screws.png" };
		size_t m_index{ 0 };
		XMUINT2 m_size{};

		Rectangle rect = { 0, 0, 31, 35 };
		EXPECT_TRUE(load->LoadTexture(m_filename, &rect, m_index, &m_size));
		EXPECT_EQ(m_index, 0);

		EXPECT_TRUE(load->LoadTexture(m_filename, nullptr, m_index, &m_size));
		EXPECT_EQ(m_index, 1);

		Rectangle rect2 = { 0, 0, 64, 64 };
		EXPECT_TRUE(load->LoadTexture(m_filename, &rect2, m_index, nullptr));
		EXPECT_EQ(m_index, 1);

		Rectangle rect3 = { 31, 35, 2, 2 };
		EXPECT_TRUE(load->LoadTexture(m_filename, &rect3, m_index, &m_size));
		EXPECT_EQ(m_index, 2);
		EXPECT_TRUE(m_size.x == 2 && m_size.y == 2);

		EXPECT_TRUE(load->LoadTexture(m_filename, &rect, m_index, &m_size));
		EXPECT_EQ(m_index, 0);

		return true;
	}

	TEST_F(CoreTest, TextureLoading)
	{
		auto texIndexing = make_unique<TextureIndexing>(
			m_deviceResources->GetD3DDevice(), m_resourceDescriptors.get(), m_batch.get(), m_spriteBatch.get());

		EXPECT_TRUE(LoadResources(LoadTexture, texIndexing.get()));
	}

	bool LoadFont(ILoadData* load)
	{
		wstring m_hangleFilename{ L"Resources/UI/Font/HangleS16.spritefont" };
		wstring m_englishFilename{ L"Resources/UI/Font/CourierNewBoldS18.spritefont" };

		size_t fontIndex1{ 0 };
		load->LoadFont(m_hangleFilename, fontIndex1);
		EXPECT_EQ(fontIndex1, 0);

		size_t fontIndex2{ 0 };
		load->LoadFont(m_hangleFilename, fontIndex2);
		EXPECT_EQ(fontIndex1, fontIndex2);

		size_t fontIndex3{ 0 };
		load->LoadFont(m_englishFilename, fontIndex3);
		EXPECT_NE(fontIndex3, 0);
		EXPECT_NE(fontIndex3, fontIndex2);

		return true;
	}

	TEST_F(CoreTest, FontLoading)
	{
		auto texIndexing = make_unique<TextureIndexing>(
			m_deviceResources->GetD3DDevice(), m_resourceDescriptors.get(), m_batch.get(), m_spriteBatch.get());

		EXPECT_TRUE(LoadResources(LoadFont, texIndexing.get()));
	}

	TEST(CoreUtility, CycleIterator)
	{
		CycleIterator addIter(1, 7);
		for (int a = 1; a != 7; ++a)
			addIter.Increase();
		EXPECT_EQ(addIter.GetCurrent(), 1);

		CycleIterator subIter(-1, 7);
		for (int a = 1; a != 2; ++a)
			subIter.Decrease();
		EXPECT_EQ(subIter.GetCurrent(), 6);
	}
}