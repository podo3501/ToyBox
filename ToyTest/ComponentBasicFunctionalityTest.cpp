#include "pch.h"
#include "ToyTestFixture.h"
#include "IMockRenderer.h"
#include "TestHelper.h"
#include "Utility.h"
#include "../Toy/Window.h"
#include "../Toy/UserInterface/UIUtility.h"
#include "../Toy/UserInterface/UIType.h"
#include "../Toy/UserInterface/Component/ImageGrid1.h"
#include "../Toy/UserInterface/Component/ImageGrid3.h"
#include "../Toy/UserInterface/Component/ImageGrid9.h"
#include "../Toy/UserInterface/Component/Button.h"
#include "../Toy/UserInterface/Component/Panel.h"
#include "../Toy/UserInterface/Component/TextArea.h"

using testing::ElementsAre;

namespace ComponentTest
{
	static void CloneTest(UIComponent* component, function<void(size_t, const RECT&, const RECT*)> renderFunc, int times)
	{
		unique_ptr<UIComponent> clonePanel = component->Clone();
		clonePanel->RefreshPosition();

		CallMockRender(clonePanel.get(), renderFunc, times);
		EXPECT_TRUE(WriteReadTest(clonePanel));
	}

	static void TestImageGrid1Render(size_t index, const RECT& dest, const RECT* source)
	{
		EXPECT_TRUE(index == 0);
		EXPECT_TRUE(IsTrue(dest, { 400, 300, 464, 364 }, *source, { 0, 0, 64, 64 }));
	}

	TEST_F(BasicFunctionalityTest, TestImageGrid1)
	{
		UILayout layout({ 64, 64 }, Origin::LeftTop);
		ImageSource grid1Source{ L"UI/Blue/button_square_header_large_square_screws.png", { { 0, 0, 64, 64 } } };

		m_panel->AttachComponent(CreateImageGrid<ImageGrid1>("ImgGrid1", layout, grid1Source), { 400, 300 });
		EXPECT_TRUE(m_renderer->LoadComponent(m_panel.get()));

		CallMockRender(m_panel.get(), TestImageGrid1Render, 1);
		EXPECT_TRUE(WriteReadTest(m_panel));

		CloneTest(m_panel.get(), TestImageGrid1Render, 1);
	}

	////////////////////////////////////////////////////////

	static void TestImageGrid3Render(size_t index, const RECT& dest, const RECT* source)
	{
		EXPECT_TRUE(index == 0);

		auto testResult{ false };
		testResult |= IsTrue(dest, { 400, 300, 430, 336 }, *source, { 10, 10, 40, 46 });
		testResult |= IsTrue(dest, { 430, 300, 470, 336 }, *source, { 40, 10, 44, 46 });
		testResult |= IsTrue(dest, { 470, 300, 500, 336 }, *source, { 44, 10, 74, 46 });

		EXPECT_TRUE(testResult);
	}

	static void TestImageGrid3ChangeAreaRender(size_t index, const RECT& dest, const RECT* source)
	{
		EXPECT_TRUE(index == 0);

		auto testResult{ false };
		testResult |= IsTrue(dest, { 340, 282, 370, 318 }, *source, { 10, 10, 40, 46 });
		testResult |= IsTrue(dest, { 370, 282, 430, 318 }, *source, { 40, 10, 44, 46 });
		testResult |= IsTrue(dest, { 430, 282, 460, 318 }, *source, { 44, 10, 74, 46 });

		EXPECT_TRUE(testResult);
	}

	static void TestImageGrid3SourceAndDivider(size_t index, const RECT& dest, const RECT* source)
	{
		EXPECT_TRUE(index == 0);

		auto testResult{ false };
		testResult |= IsTrue(dest, { 340, 282, 370, 318 }, *source, { 10, 10, 30, 46 });
		testResult |= IsTrue(dest, { 370, 282, 430, 318 }, *source, { 30, 10, 54, 46 });
		testResult |= IsTrue(dest, { 430, 282, 460, 318 }, *source, { 54, 10, 74, 46 });

		EXPECT_TRUE(testResult);
	}

	TEST_F(BasicFunctionalityTest, TestImageGrid3)
	{
		UILayout layout({ 100, 36 }, Origin::LeftTop);
		ImageSource grid3Source{
			L"UI/Blue/button_square_header_large_square_screws.png", {
				{ 10, 10, 30, 36 }, { 40, 10, 4, 36 }, { 44, 10, 30, 36 }
			}
		};

		m_panel->AttachComponent(CreateImageGrid<ImageGrid3>("ImgGrid3", layout, grid3Source), { 400, 300 });
		EXPECT_TRUE(m_renderer->LoadComponent(m_panel.get()));

		CallMockRender(m_panel.get(), TestImageGrid3Render, 3);

		ImageGrid3* img3 = nullptr;
		m_panel->GetComponent("ImgGrid3", &img3);
		img3->ChangeOrigin(Origin::Center);
		img3->ChangeSize({ 120, 36 });
		img3->RefreshPosition();

		CallMockRender(m_panel.get(), TestImageGrid3ChangeAreaRender, 3);
		EXPECT_TRUE(WriteReadTest(m_panel));

		CloneTest(m_panel.get(), TestImageGrid3ChangeAreaRender, 3);

		SourceDivider srcDivider{};
		img3->GetSourceAnd2Divider(srcDivider);

		EXPECT_TRUE((srcDivider.rect == Rectangle{ 10, 10, 64, 36 }));
		EXPECT_THAT(srcDivider.list, ElementsAre(30, 34));

		srcDivider.list.clear();
		srcDivider.list = { 20, 44 };
		img3->SetSourceAnd2Divider(srcDivider);

		CallMockRender(m_panel.get(), TestImageGrid3SourceAndDivider, 3);
	}

	////////////////////////////////////////////////////////

	static void TestImageGrid9Render(size_t index, const RECT& dest, const RECT* source)
	{
		EXPECT_TRUE(index == 0);

		auto testResult{ false };
		testResult |= IsTrue(dest, { 400, 300, 430, 336 }, *source, { 10, 10, 40, 46 });
		testResult |= IsTrue(dest, { 430, 300, 540, 336 }, *source, { 40, 10, 44, 46 });
		testResult |= IsTrue(dest, { 540, 300, 570, 336 }, *source, { 44, 10, 74, 46 });

		testResult |= IsTrue(dest, { 400, 336, 430, 394 }, *source, { 10, 46, 40, 48 });
		testResult |= IsTrue(dest, { 430, 336, 540, 394 }, *source, { 40, 46, 44, 48 });
		testResult |= IsTrue(dest, { 540, 336, 570, 394 }, *source, { 44, 46, 74, 48 });

		testResult |= IsTrue(dest, { 400, 394, 430, 420 }, *source, { 10, 48, 40, 74 });
		testResult |= IsTrue(dest, { 430, 394, 540, 420 }, *source, { 40, 48, 44, 74 });
		testResult |= IsTrue(dest, { 540, 394, 570, 420 }, *source, { 44, 48, 74, 74 });

		EXPECT_TRUE(testResult);
	}

	static void TestImageGrid9ChangeAreaRender(size_t index, const RECT& dest, const RECT* source)
	{
		EXPECT_TRUE(index == 0);

		auto testResult{ false };
		testResult |= IsTrue(dest, { 310, 225, 340, 261 }, *source, { 10, 10, 40, 46 });
		testResult |= IsTrue(dest, { 340, 225, 460, 261 }, *source, { 40, 10, 44, 46 });
		testResult |= IsTrue(dest, { 460, 225, 490, 261 }, *source, { 44, 10, 74, 46 });

		testResult |= IsTrue(dest, { 310, 261, 340, 349 }, *source, { 10, 46, 40, 48 });
		testResult |= IsTrue(dest, { 340, 261, 460, 349 }, *source, { 40, 46, 44, 48 });
		testResult |= IsTrue(dest, { 460, 261, 490, 349 }, *source, { 44, 46, 74, 48 });

		testResult |= IsTrue(dest, { 310, 349, 340, 375 }, *source, { 10, 48, 40, 74 });
		testResult |= IsTrue(dest, { 340, 349, 460, 375 }, *source, { 40, 48, 44, 74 });
		testResult |= IsTrue(dest, { 460, 349, 490, 375 }, *source, { 44, 48, 74, 74 });

		EXPECT_TRUE(testResult);
	}

	static void TestImageGrid9SourceAndDivider(size_t index, const RECT& dest, const RECT* source)
	{
		EXPECT_TRUE(index == 0);

		auto testResult{ false };
		testResult |= IsTrue(dest, { 310, 225, 340, 261 }, *source, { 10, 10, 30, 36 });
		testResult |= IsTrue(dest, { 340, 225, 460, 261 }, *source, { 30, 10, 54, 36 });
		testResult |= IsTrue(dest, { 460, 225, 490, 261 }, *source, { 54, 10, 74, 36 });

		testResult |= IsTrue(dest, { 310, 261, 340, 349 }, *source, { 10, 36, 30, 58 });
		testResult |= IsTrue(dest, { 340, 261, 460, 349 }, *source, { 30, 36, 54, 58 });
		testResult |= IsTrue(dest, { 460, 261, 490, 349 }, *source, { 54, 36, 74, 58 });

		testResult |= IsTrue(dest, { 310, 349, 340, 375 }, *source, { 10, 58, 30, 74 });
		testResult |= IsTrue(dest, { 340, 349, 460, 375 }, *source, { 30, 58, 54, 74 });
		testResult |= IsTrue(dest, { 460, 349, 490, 375 }, *source, { 54, 58, 74, 74 });

		EXPECT_TRUE(testResult);
	}

	TEST_F(BasicFunctionalityTest, TestImageGrid9)
	{
		UILayout layout({ 170, 120 }, Origin::LeftTop);
		ImageSource grid9Source{
			L"UI/Blue/button_square_header_large_square_screws.png", {
				{ 10, 10, 30, 36 }, { 40, 10, 4, 36 }, { 44, 10, 30, 36 },
				{ 10, 46, 30, 2 }, { 40, 46, 4, 2 }, { 44, 46, 30, 2 },
				{ 10, 48, 30, 26 }, { 40, 48, 4, 26 }, { 44, 48, 30, 26 }
			}
		};

		m_panel->AttachComponent(CreateImageGrid<ImageGrid9>("ImgGrid9", layout, grid9Source), { 400, 300 });
		EXPECT_TRUE(m_renderer->LoadComponent(m_panel.get()));

		CallMockRender(m_panel.get(), TestImageGrid9Render, 9);

		ImageGrid9* img9 = nullptr;
		m_panel->GetComponent("ImgGrid9", &img9);
		img9->ChangeOrigin(Origin::Center);
		img9->ChangeSize({ 180, 150 });
		img9->RefreshPosition();

		CallMockRender(m_panel.get(), TestImageGrid9ChangeAreaRender, 9);
		EXPECT_TRUE(WriteReadTest(m_panel));

		CloneTest(m_panel.get(), TestImageGrid9ChangeAreaRender, 9);

		SourceDivider srcDivider{};
		img9->GetSourceAnd4Divider(srcDivider);

		EXPECT_TRUE((srcDivider.rect == Rectangle{ 10, 10, 64, 64 }));
		EXPECT_THAT(srcDivider.list, ElementsAre(30, 34, 36, 38));

		srcDivider.list.clear();
		srcDivider.list = { 20, 44, 26, 48 };
		img9->SetSourceAnd4Divider(srcDivider);

		CallMockRender(m_panel.get(), TestImageGrid9SourceAndDivider, 9);
	}

	/////////////////////////////////////////////////////////////////////////

	static void TestButton_ImageGrid1Render(size_t index, const RECT& dest, const RECT* source)
	{
		EXPECT_TRUE(index == 0);

		auto testResult{ false };
		testResult |= IsTrue(dest, { 144, 104, 176, 136 }, *source, { 0, 0, 32, 32 });
		testResult |= IsTrue(dest, { 144, 104, 176, 136 }, *source, { 35, 0, 67, 32 });
		testResult |= IsTrue(dest, { 144, 104, 176, 136 }, *source, { 70, 0, 102, 32 });

		EXPECT_TRUE(testResult);
	}

	TEST_F(BasicFunctionalityTest, TestButton_ImageGrid1)
	{
		UILayout loButton({ 32, 32 }, Origin::Center);
		UILayout loImgGrid({ 32, 32 }, Origin::LeftTop);
		ImageSource normal{ { L"UI/Texture/Test_01.png" }, { { 0, 0, 32, 32} } };
		ImageSource hover{ { L"UI/Texture/Test_01.png" }, { { 35, 0, 32, 32} } };
		ImageSource pressed{ { L"UI/Texture/Test_01.png" }, { { 70, 0, 32, 32} } };

		std::unique_ptr<Button> button = std::make_unique<Button>();
		EXPECT_TRUE(button->SetImage("Button", loButton,
			CreateImageGrid<ImageGrid1>("Button_normal", loImgGrid, normal),
			CreateImageGrid<ImageGrid1>("Button_hover", loImgGrid, hover),
			CreateImageGrid<ImageGrid1>("Button_pressed", loImgGrid, pressed)));

		m_panel->AttachComponent(move(button), { 160, 120 });
		EXPECT_TRUE(m_renderer->LoadComponent(m_panel.get()));

		TestUpdate(m_window->GetHandle(), m_panel.get(), 144, 120);	//hover
		CallMockRender(m_panel.get(), TestButton_ImageGrid1Render, 1);
		//EXPECT_TRUE(WriteReadTest(m_panel));

		//CloneTest(m_panel.get(), TestButton_ImageGrid1Render, 1);	//normal
	}

	static void TestButton_ImageGrid3Render(size_t index, const RECT& dest, const RECT* source)
	{
		EXPECT_TRUE(index == 0);

		auto testResult{ false };
		testResult |= IsTrue(dest, { 110, 96, 132, 144 }, *source, { 118, 35, 140, 83 });
		testResult |= IsTrue(dest, { 132, 96, 188, 144 }, *source, { 140, 35, 144, 83 });
		testResult |= IsTrue(dest, { 188, 96, 210, 144 }, *source, { 144, 35, 166, 83 });

		EXPECT_TRUE(testResult);
	}

	static void TestButton_ImageGrid3ChangeAreaRender(size_t index, const RECT& dest, const RECT* source)
	{
		EXPECT_TRUE(index == 0);

		auto testResult{ false };
		testResult |= IsTrue(dest, { 85, 96, 107, 144 }, *source, { 67, 35, 89, 83 });
		testResult |= IsTrue(dest, { 107, 96, 213, 144 }, *source, { 89, 35, 93, 83 });
		testResult |= IsTrue(dest, { 213, 96, 235, 144 }, *source, { 93, 35, 115, 83 });

		EXPECT_TRUE(testResult);
	}

	TEST_F(BasicFunctionalityTest, TestButton_ImageGrid3)
	{
		UILayout loButton({ 100, 48 }, Origin::Center);
		UILayout loImgGrid({ 100, 48 }, Origin::LeftTop);
		ImageSource normal{ { L"UI/Texture/Test_01.png" }, { { 67, 35, 22, 48}, { 89, 35, 4, 48 }, { 93, 35, 22, 48 } } };
		ImageSource hover{ { L"UI/Texture/Test_01.png" }, { { 118, 35, 22, 48}, { 140, 35, 4, 48 }, { 144, 35, 22, 48 } } };
		ImageSource pressed{ { L"UI/Texture/Test_01.png" }, { { 169, 35, 22, 48}, { 191, 35, 4, 48 }, { 195, 35, 22, 48 } } };

		std::unique_ptr<Button> button = std::make_unique<Button>();
		EXPECT_TRUE(button->SetImage("Button", loButton,
			CreateImageGrid<ImageGrid3>("Button_normal", loImgGrid, normal),
			CreateImageGrid<ImageGrid3>("Button_hover", loImgGrid, hover),
			CreateImageGrid<ImageGrid3>("Button_pressed", loImgGrid, pressed)));

		m_panel->AttachComponent(move(button), { 160, 120 });
		EXPECT_TRUE(m_renderer->LoadComponent(m_panel.get()));

		TestUpdate(m_window->GetHandle(), m_panel.get(), 110, 96);	//Hover
		CallMockRender(m_panel.get(), TestButton_ImageGrid3Render, 3);

		Button* btn = nullptr;
		m_panel->GetComponent("Button", &btn);
		btn->ChangeSize({ 150, 48 });
		TestUpdate(m_window->GetHandle(), m_panel.get(), 0, 0);	//Normal

		CallMockRender(m_panel.get(), TestButton_ImageGrid3ChangeAreaRender, 3);
		EXPECT_TRUE(WriteReadTest(m_panel));

		CloneTest(m_panel.get(), TestButton_ImageGrid3ChangeAreaRender, 3);
	}

	static void TestTextAreaRender(size_t index, const wstring& text, const Vector2& pos, const FXMVECTOR& color)
	{
		if (text == L"테스") EXPECT_TRUE(index == 1 && pos == Vector2(240.f, 240.f) && DirectX::XMVector4Equal(color, Colors::Red));
		if (text == L"테스트2") EXPECT_TRUE(index == 1 && pos == Vector2(282, 268.375f) && DirectX::XMVector4Equal(color, Colors::Red));
		if (text == L"^") EXPECT_TRUE(index == 0 && pos == Vector2(531.f, 268.375f) && DirectX::XMVector4Equal(color, Colors::Black));
		if (text == L"&*") EXPECT_TRUE(index == 0 && pos == Vector2(240.f, 296.75f) && DirectX::XMVector4Equal(color, Colors::Blue));
	}

	TEST_F(BasicFunctionalityTest, TestTextArea)
	{
		std::unique_ptr<TextArea> textArea = std::make_unique<TextArea>();
		UILayout layout({ 320, 120 }, Origin::Center);
		map<wstring, wstring> fontFileList;
		fontFileList.insert(make_pair(L"Hangle", L"UI/Font/MaleunGothicS16.spritefont"));
		fontFileList.insert(make_pair(L"English", L"UI/Font/CourierNewBoldS18.spritefont"));
		wstring text = L"<Hangle><Red>테스<br>트, 테스트2</Red>!@#$% </Hangle><English>Test. ^<Blue>&*</Blue>() End</English>";
		textArea->SetFont("TextArea", text, layout, fontFileList);

		m_panel->AttachComponent(move(textArea), { 400, 300 });
		EXPECT_TRUE(m_renderer->LoadComponent(m_panel.get()));

		CallMockRender(m_panel.get(), TestTextAreaRender);
		EXPECT_TRUE(WriteReadTest(m_panel));

		unique_ptr<UIComponent> clonePanel = m_panel->Clone();
		clonePanel->RefreshPosition();

		CallMockRender(clonePanel.get(), TestTextAreaRender);
		EXPECT_TRUE(WriteReadTest(clonePanel));
	}

	TEST_F(BasicFunctionalityTest, TestRecursivePosition)
	{
		std::unique_ptr<Panel> panel2 = std::make_unique<Panel>();
		panel2->SetLayout({ { 20, 20 }, Origin::Center });
		Panel* ptrPanel = panel2.get();

		std::unique_ptr<Panel> panel1 = std::make_unique<Panel>();
		panel1->SetLayout({ { 400, 400 }, Origin::Center });

		panel1->AttachComponent(move(panel2), { 40, 40 });
		m_panel->AttachComponent(move(panel1), { 400, 300 });

		vector<UIComponent*> outList;
		m_panel->GetComponents({ 240, 140 }, outList);
		EXPECT_EQ(outList.size(), 3);

		ptrPanel->ChangeOrigin(Origin::LeftTop);

		outList.clear();
		m_panel->GetComponents({ 239, 140 }, outList);
		EXPECT_EQ(outList.size(), 2);

		//사이즈가 바뀌었을때 값이 어떻게 바뀌는지 테스트
	}
}

