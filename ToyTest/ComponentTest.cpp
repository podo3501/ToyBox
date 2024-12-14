#include "pch.h"
#include "ToyTestFixture.h"
#include "IMockRenderer.h"
#include "TestHelper.h"
#include "Utility.h"
#include "../Toy/UserInterface/UIUtility.h"
#include "../Toy/UserInterface/UIType.h"
#include "../Toy/UserInterface/UILayout.h"
#include "../Toy/UserInterface/UIComponent.h"
#include "../Toy/UserInterface/ImageGrid1.h"
#include "../Toy/UserInterface/ImageGrid3.h"
#include "../Toy/UserInterface/ImageGrid9.h"
#include "../Toy/UserInterface/BGImage.h"

class Button : public UIComponent
{
public:
	bool SetImage(const string& name, const UILayout& layout,
		unique_ptr<UIComponent>&& normal,
		unique_ptr<UIComponent>&& hover,
		unique_ptr<UIComponent>&& pressed)
	{
		SetName(name);
		SetLayout(layout);

		m_images.emplace(ButtonState::Normal, move(normal));
		m_images.emplace(ButtonState::Hover, move(hover));
		m_images.emplace(ButtonState::Pressed, move(pressed));

		return true;
	}

private:
	map<ButtonState, unique_ptr<UIComponent>> m_images;
};

using ::testing::_;
using ::testing::Invoke;

namespace ComponentTest
{
	void TestImageGrid1Render(size_t index, const RECT& dest, const RECT* source, bool selected)
	{
		EXPECT_TRUE(index == 0);
		EXPECT_TRUE(IsTrue(dest, { 400, 300, 464, 364 }, *source, { 0, 0, 64, 64 }));
	}

	TEST_F(ToyTestFixture, ImageGrid1Test)
	{
		UILayout layout({ 0, 0, 64, 64 }, Origin::LeftTop);
		ImageSource grid1Source{ L"UI/Blue/button_square_header_large_square_screws.png", { { 0, 0, 64, 64 } } };

		m_panel->AddComponent(CreateGrid1("ImgGrid1", layout, grid1Source), { 0.5f, 0.5f });
		EXPECT_TRUE(m_renderer->LoadComponents());

		CallMockRender(m_panel.get(), TestImageGrid1Render);

		EXPECT_TRUE(WriteReadTest(m_panel));
	}

	////////////////////////////////////////////////////////

	void TestImageGrid3Render(size_t index, const RECT& dest, const RECT* source, bool selected)
	{
		EXPECT_TRUE(index == 0);

		auto testResult{ false };
		testResult |= IsTrue(dest, { 400, 300, 430, 336 }, *source, { 0, 0, 30, 36 });
		testResult |= IsTrue(dest, { 430, 300, 470, 336 }, *source, { 30, 0, 34, 36 });
		testResult |= IsTrue(dest, { 470, 300, 500, 336 }, *source, { 34, 0, 64, 36 });
		
		EXPECT_TRUE(testResult);
	}

	TEST_F(ToyTestFixture, ImageGrid3Test)
	{
		UILayout layout({ 0, 0, 100, 36 }, Origin::LeftTop);
		ImageSource grid3Source{
			L"UI/Blue/button_square_header_large_square_screws.png", {
				{ 0, 0, 30, 36 }, { 30, 0, 4, 36 }, { 34, 0, 30, 36 }
			}
		};
		unique_ptr<ImageGrid3> imgGrid3 = make_unique<ImageGrid3>();
		imgGrid3->SetImage("ImgGrid3", layout, grid3Source, false);

		m_panel->AddComponent(move(imgGrid3), { 0.5f, 0.5f });
		EXPECT_TRUE(m_renderer->LoadComponents());

		CallMockRender(m_panel.get(), TestImageGrid3Render);

		ImageGrid3* grid3 = nullptr;
		m_panel->GetComponent("ImgGrid3", &grid3);
		
		EXPECT_TRUE(WriteReadTest(m_panel));
	}

	////////////////////////////////////////////////////////

	void TestImageGrid9Render(size_t index, const RECT& dest, const RECT* source, bool selected)
	{
		EXPECT_TRUE(index == 0);

		auto testResult{ false };
		testResult |= IsTrue(dest, { 400, 300, 430, 336 }, *source, { 0, 0, 30, 36 });
		testResult |= IsTrue(dest, { 430, 300, 540, 336 }, *source, { 30, 0, 34, 36 });
		testResult |= IsTrue(dest, { 540, 300, 570, 336 }, *source, { 34, 0, 64, 36 });

		testResult |= IsTrue(dest, { 400, 336, 430, 394 }, *source, { 0, 36, 30, 38 });
		testResult |= IsTrue(dest, { 430, 336, 540, 394 }, *source, { 30, 36, 34, 38 });
		testResult |= IsTrue(dest, { 540, 336, 570, 394 }, *source, { 34, 36, 64, 38 });

		testResult |= IsTrue(dest, { 400, 394, 430, 420 }, *source, { 0, 38, 30, 64 });
		testResult |= IsTrue(dest, { 430, 394, 540, 420 }, *source, { 30, 38, 34, 64 });
		testResult |= IsTrue(dest, { 540, 394, 570, 420 }, *source, { 34, 38, 64, 64 });

		EXPECT_TRUE(testResult);
	}

	TEST_F(ToyTestFixture, ImageGrid9Test)
	{
		UILayout layout({ 0, 0, 170, 120 }, Origin::LeftTop);
		ImageSource grid9Source{
			L"UI/Blue/button_square_header_large_square_screws.png", {
				{ 0, 0, 30, 36 }, { 30, 0, 4, 36 }, { 34, 0, 30, 36 },
				{ 0, 36, 30, 2 }, { 30, 36, 4, 2 }, { 34, 36, 30, 2 },
				{ 0, 38, 30, 26 }, { 30, 38, 4, 26 }, { 34, 38, 30, 26 }
			}
		};

		unique_ptr<ImageGrid9> imgGrid9 = make_unique<ImageGrid9>();
		imgGrid9->SetImage("ImgGrid9", layout, grid9Source);

		m_panel->AddComponent(move(imgGrid9), { 0.5f, 0.5f });
		EXPECT_TRUE(m_renderer->LoadComponents());

		CallMockRender(m_panel.get(), TestImageGrid9Render);

		EXPECT_TRUE(WriteReadTest(m_panel));
	}

	TEST_F(ToyTestFixture, ButtonTest)
	{
		UILayout loButton({ 0, 0, 32, 32 }, Origin::Center);
		UILayout loImgGrid({ 0, 0, 32, 32 }, Origin::LeftTop);
		ImageSource normal{ { L"UI/Texture/Test_01.png" }, { { 0, 0, 32, 32} } };
		ImageSource hover{ { L"UI/Texture/Test_01.png" }, { { 35, 0, 32, 32} } };
		ImageSource pressed{ { L"UI/Texture/Test_01.png" }, { { 70, 0, 32, 32} } };

		std::unique_ptr<Button> button = std::make_unique<Button>();
		button->SetImage("Button", loButton,
			CreateGrid1("Button_normal", loImgGrid, normal),
			CreateGrid1("Button_hover", loImgGrid, hover),
			CreateGrid1("Button_pressed", loImgGrid, pressed));
	}
}