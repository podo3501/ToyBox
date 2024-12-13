#include "pch.h"
#include "ToyTestFixture.h"
#include "IMockRenderer.h"
#include "TestHelper.h"
#include "../Toy/UserInterface/UIUtility.h"
#include "../Toy/UserInterface/UIType.h"
#include "../Toy/UserInterface/UILayout.h"
#include "../Toy/UserInterface/ImageGrid1.h"
#include "../Toy/UserInterface/ImageGrid3.h"

using ::testing::_;
using ::testing::Invoke;

namespace ComponentTest
{
	void TestImageGrid1Render(size_t index, const RECT& dest, const RECT* source, bool selected)
	{
		EXPECT_TRUE(index == 0);
		EXPECT_TRUE(source->right == 64 && source->bottom == 64);
		EXPECT_TRUE(dest.right == 464 && dest.bottom == 364);

		return;
	}

	TEST_F(ToyTestFixture, ImageGrid1Test)
	{
		UILayout layout({ 0, 0, 64, 64 }, Origin::LeftTop);
		unique_ptr<ImageGrid1> imgGrid1 = make_unique<ImageGrid1>();
		ImageSource grid1Source{
			L"UI/Blue/button_square_header_large_square_screws.png", {
				{ 0, 0, 64, 64 }
			}
		};
		EXPECT_TRUE(imgGrid1->SetImage("ImgGrid1", layout, grid1Source));

		m_panel->AddComponent(move(imgGrid1), { 0.5f, 0.5f });
		m_renderer->LoadComponents();

		m_panel->Update({}, nullptr);
		CallMockRender(m_panel.get(), TestImageGrid1Render);

		EXPECT_TRUE(WriteReadTest(m_panel));
	}

	////////////////////////////////////////////////////////


	void TestImageGrid3Render(size_t index, const RECT& dest, const RECT* source, bool selected)
	{
		/*EXPECT_TRUE(index == 0);
		EXPECT_TRUE(source->right == 64 && source->bottom == 64);
		EXPECT_TRUE(dest.right == 464 && dest.bottom == 364);*/

		return;
	}

	TEST_F(ToyTestFixture, ImageGrid3Test)
	{
		UILayout layout({ 0, 0, 100, 36 }, Origin::LeftTop);
		unique_ptr<ImageGrid3> imgGrid3 = make_unique<ImageGrid3>();
		ImageSource grid3Source{
			L"UI/Blue/button_square_header_large_square_screws.png", {
				{ 0, 0, 30, 36 }, { 30, 0, 4, 36 }, { 34, 0, 30, 36 }
			}
		};
		imgGrid3->SetImage("ImgGrid3", layout, grid3Source);

		m_panel->AddComponent(move(imgGrid3), { 0.5f, 0.5f });
		m_renderer->LoadComponents();

		m_panel->Update({}, nullptr);
		CallMockRender(m_panel.get(), TestImageGrid3Render);

		EXPECT_TRUE(WriteReadTest(m_panel));
	}
}