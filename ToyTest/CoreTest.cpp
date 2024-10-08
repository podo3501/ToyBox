#include "pch.h"
#include "CoreTestFixture.h"
#include "../Core/TextureIndexing.h"
#include "../Core/DeviceResources.h"

bool MockLoadResources(ILoadData* load)
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

	EXPECT_TRUE(LoadResources(MockLoadResources, texIndexing.get()));	
}

TEST_F(CoreTest, TextureIndexing)
{

}