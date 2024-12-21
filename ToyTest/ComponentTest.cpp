#include "pch.h"
#include "ToyTestFixture.h"
#include "IMockRenderer.h"
#include "TestHelper.h"
#include "Utility.h"
#include "../Toy/Window.h"
#include "../Toy/UserInterface/UIUtility.h"
#include "../Toy/UserInterface/UIType.h"
#include "../Toy/UserInterface/ImageGrid1.h"
#include "../Toy/UserInterface/ImageGrid3.h"
#include "../Toy/UserInterface/ImageGrid9.h"
#include "../Toy/UserInterface/Button.h"
#include "../Toy/UserInterface/Panel.h"
#include "../Toy/UserInterface/TextArea.h"

namespace ComponentTest
{
	void CloneTest(UIComponent* component, function<void(size_t, const RECT&, const RECT*, bool)> renderFunc, int times)
	{
		unique_ptr<UIComponent> clonePanel = component->Clone();
		clonePanel->ProcessUpdate({}, nullptr);

		CallMockRender(clonePanel.get(), renderFunc, times);
		EXPECT_TRUE(WriteReadTest(clonePanel));
	}

	void TestImageGrid1Render(size_t index, const RECT& dest, const RECT* source, bool selected)
	{
		EXPECT_TRUE(index == 0);
		EXPECT_TRUE(IsTrue(dest, { 400, 300, 464, 364 }, *source, { 0, 0, 64, 64 }));
	}

	TEST_F(ToyTestFixture, TestImageGrid1)
	{
		UILayout layout({ 64, 64 }, Origin::LeftTop);
		ImageSource grid1Source{ L"UI/Blue/button_square_header_large_square_screws.png", { { 0, 0, 64, 64 } } };

		m_panel->AddComponent(CreateImageGrid1("ImgGrid1", layout, grid1Source), { 0.5f, 0.5f });
		EXPECT_TRUE(m_renderer->LoadComponents());

		CallMockRender(m_panel.get(), TestImageGrid1Render, 1);
		EXPECT_TRUE(WriteReadTest(m_panel));

		CloneTest(m_panel.get(), TestImageGrid1Render, 1);
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

	void TestImageGrid3ChangeAreaRender(size_t index, const RECT& dest, const RECT* source, bool selected)
	{
		EXPECT_TRUE(index == 0);

		auto testResult{ false };
		testResult |= IsTrue(dest, { 340, 282, 370, 318 }, *source, { 0, 0, 30, 36 });
		testResult |= IsTrue(dest, { 370, 282, 430, 318 }, *source, { 30, 0, 34, 36 });
		testResult |= IsTrue(dest, { 430, 282, 460, 318 }, *source, { 34, 0, 64, 36 });

		EXPECT_TRUE(testResult);
	}

	TEST_F(ToyTestFixture, TestImageGrid3)
	{
		UILayout layout({ 100, 36 }, Origin::LeftTop);
		ImageSource grid3Source{
			L"UI/Blue/button_square_header_large_square_screws.png", {
				{ 0, 0, 30, 36 }, { 30, 0, 4, 36 }, { 34, 0, 30, 36 }
			}
		};
		unique_ptr<ImageGrid3> imgGrid3 = make_unique<ImageGrid3>();
		imgGrid3->SetImage("ImgGrid3", layout, grid3Source);

		m_panel->AddComponent(move(imgGrid3), { 0.5f, 0.5f });
		EXPECT_TRUE(m_renderer->LoadComponents());

		CallMockRender(m_panel.get(), TestImageGrid3Render, 3);

		ImageGrid3* img3 = nullptr;
		m_panel->GetComponent("ImgGrid3", &img3);
		img3->ChangeOrigin(Origin::Center);
		img3->ChangeSize({ 120, 36 });
		m_panel->ProcessUpdate({}, nullptr);	//위치값을 재계산한다. parent를 구현했기 때문에 parent로 올라가면서 계산 로직을 적용하고 이것은 삭제예정.

		CallMockRender(m_panel.get(), TestImageGrid3ChangeAreaRender, 3);		
		EXPECT_TRUE(WriteReadTest(m_panel));

		CloneTest(m_panel.get(), TestImageGrid3ChangeAreaRender, 3);
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

	void TestImageGrid9ChangeAreaRender(size_t index, const RECT& dest, const RECT* source, bool selected)
	{
		EXPECT_TRUE(index == 0);

		auto testResult{ false };
		testResult |= IsTrue(dest, { 310, 225, 340, 261 }, *source, { 0, 0, 30, 36 });
		testResult |= IsTrue(dest, { 340, 225, 460, 261 }, *source, { 30, 0, 34, 36 });
		testResult |= IsTrue(dest, { 460, 225, 490, 261 }, *source, { 34, 0, 64, 36 });

		testResult |= IsTrue(dest, { 310, 261, 340, 349 }, *source, { 0, 36, 30, 38 });
		testResult |= IsTrue(dest, { 340, 261, 460, 349 }, *source, { 30, 36, 34, 38 });
		testResult |= IsTrue(dest, { 460, 261, 490, 349 }, *source, { 34, 36, 64, 38 });

		testResult |= IsTrue(dest, { 310, 349, 340, 375 }, *source, { 0, 38, 30, 64 });
		testResult |= IsTrue(dest, { 340, 349, 460, 375 }, *source, { 30, 38, 34, 64 });
		testResult |= IsTrue(dest, { 460, 349, 490, 375 }, *source, { 34, 38, 64, 64 });

		EXPECT_TRUE(testResult);
	}

	TEST_F(ToyTestFixture, TestImageGrid9)
	{
		UILayout layout({ 170, 120 }, Origin::LeftTop);
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

		CallMockRender(m_panel.get(), TestImageGrid9Render, 9);

		ImageGrid9* img9 = nullptr;
		m_panel->GetComponent("ImgGrid9", &img9);
		img9->ChangeOrigin(Origin::Center);
		img9->ChangeSize({ 180, 150 });
		m_panel->ProcessUpdate({}, nullptr);	//위치값을 재계산한다.

		CallMockRender(m_panel.get(), TestImageGrid9ChangeAreaRender, 9);
		EXPECT_TRUE(WriteReadTest(m_panel));

		CloneTest(m_panel.get(), TestImageGrid9ChangeAreaRender, 9);
	}

	void TestButton_ImageGrid1Render(size_t index, const RECT& dest, const RECT* source, bool selected)
	{
		EXPECT_TRUE(index == 0);

		auto testResult{ false };
		testResult |= IsTrue(dest, { 144, 104, 176, 136 }, *source, { 0, 0, 32, 32 });
		testResult |= IsTrue(dest, { 144, 104, 176, 136 }, *source, { 35, 0, 67, 32 });
		testResult |= IsTrue(dest, { 144, 104, 176, 136 }, *source, { 70, 0, 102, 32 });

		EXPECT_TRUE(testResult);
	}

	TEST_F(ToyTestFixture, TestButton_ImageGrid1)
	{
		UILayout loButton({ 32, 32 }, Origin::Center);
		UILayout loImgGrid({ 32, 32 }, Origin::LeftTop);
		ImageSource normal{ { L"UI/Texture/Test_01.png" }, { { 0, 0, 32, 32} } };
		ImageSource hover{ { L"UI/Texture/Test_01.png" }, { { 35, 0, 32, 32} } };
		ImageSource pressed{ { L"UI/Texture/Test_01.png" }, { { 70, 0, 32, 32} } };

		std::unique_ptr<Button> button = std::make_unique<Button>();
		EXPECT_TRUE(button->SetImage("Button", loButton,
			CreateImageGrid1("Button_normal", loImgGrid, normal),
			CreateImageGrid1("Button_hover", loImgGrid, hover),
			CreateImageGrid1("Button_pressed", loImgGrid, pressed)));

		m_panel->AddComponent(move(button), { 0.2f, 0.2f });
		EXPECT_TRUE(m_renderer->LoadComponents());

		TestUpdate(m_window->GetHandle(), m_panel.get(), 144, 120 );	//Pressed
		CallMockRender(m_panel.get(), TestButton_ImageGrid1Render, 1);
		EXPECT_TRUE(WriteReadTest(m_panel));

		CloneTest(m_panel.get(), TestButton_ImageGrid1Render, 1);
	}

	void TestButton_ImageGrid3Render(size_t index, const RECT& dest, const RECT* source, bool selected)
	{
		EXPECT_TRUE(index == 0);

		auto testResult{ false };
		testResult |= IsTrue(dest, { 110, 96, 132, 144 }, *source, { 169, 35, 191, 83 });
		testResult |= IsTrue(dest, { 132, 96, 188, 144 }, *source, { 191, 35, 195, 83 });
		testResult |= IsTrue(dest, { 188, 96, 210, 144 }, *source, { 195, 35, 217, 83 });

		EXPECT_TRUE(testResult);
	}

	void TestButton_ImageGrid3ChangeAreaRender(size_t index, const RECT& dest, const RECT* source, bool selected)
	{
		EXPECT_TRUE(index == 0);

		auto testResult{ false };
		testResult |= IsTrue(dest, { 85, 96, 107, 144 }, *source, { 169, 35, 191, 83 });
		testResult |= IsTrue(dest, { 107, 96, 213, 144 }, *source, { 191, 35, 195, 83 });
		testResult |= IsTrue(dest, { 213, 96, 235, 144 }, *source, { 195, 35, 217, 83 });

		EXPECT_TRUE(testResult);
	}

	TEST_F(ToyTestFixture, TestButton_ImageGrid3)
	{
		UILayout loButton({ 100, 48 }, Origin::Center);
		UILayout loImgGrid({ 100, 48 }, Origin::LeftTop);
		ImageSource normal{ { L"UI/Texture/Test_01.png" }, { { 67, 35, 22, 48}, { 89, 35, 4, 48 }, { 93, 35, 22, 48 } } };
		ImageSource hover{ { L"UI/Texture/Test_01.png" }, { { 118, 35, 22, 48}, { 140, 35, 4, 48 }, { 144, 35, 22, 48 } } };
		ImageSource pressed{ { L"UI/Texture/Test_01.png" }, { { 169, 35, 22, 48}, { 191, 35, 4, 48 }, { 195, 35, 22, 48 } } };

		std::unique_ptr<Button> button = std::make_unique<Button>();
		EXPECT_TRUE(button->SetImage("Button", loButton,
			CreateImageGrid3("Button_normal", loImgGrid, normal),
			CreateImageGrid3("Button_hover", loImgGrid, hover),
			CreateImageGrid3("Button_pressed", loImgGrid, pressed)));

		m_panel->AddComponent(move(button), { 0.2f, 0.2f });
		EXPECT_TRUE(m_renderer->LoadComponents());

		TestUpdate(m_window->GetHandle(), m_panel.get(), 110, 96);	//Pressed
		CallMockRender(m_panel.get(), TestButton_ImageGrid3Render, 3);

		Button* btn = nullptr;
		m_panel->GetComponent("Button", &btn);
		btn->ChangeSize({ 150, 48 });
		m_panel->ProcessUpdate({}, nullptr);	//위치값을 재계산한다.

		CallMockRender(m_panel.get(), TestButton_ImageGrid3ChangeAreaRender, 3);
		EXPECT_TRUE(WriteReadTest(m_panel));

		CloneTest(m_panel.get(), TestButton_ImageGrid3ChangeAreaRender, 3);
	}

	void TestTextAreaRender(size_t index, const wstring& text, const Vector2& pos, const FXMVECTOR& color)
	{
		if (text == L"테스") EXPECT_TRUE(index == 1 && pos == Vector2(240.f, 240.f) && DirectX::XMVector4Equal(color, Colors::Red));
		if (text == L"테스트2") EXPECT_TRUE(index == 1 && pos == Vector2(282, 268.375f) && DirectX::XMVector4Equal(color, Colors::Red));
		if (text == L"^") EXPECT_TRUE(index == 0 && pos == Vector2(531.f, 268.375f) && DirectX::XMVector4Equal(color, Colors::Black));
		if (text == L"&*") EXPECT_TRUE(index == 0 && pos == Vector2(240.f, 296.75f) && DirectX::XMVector4Equal(color, Colors::Blue));
	}

	TEST_F(ToyTestFixture, TestTextArea)
	{
		std::unique_ptr<TextArea> textArea = std::make_unique<TextArea>();
		UILayout layout({ 320, 120 }, Origin::Center);
		map<wstring, wstring> fontFileList;
		fontFileList.insert(make_pair(L"Hangle", L"UI/Font/MaleunGothicS16.spritefont"));
		fontFileList.insert(make_pair(L"English", L"UI/Font/CourierNewBoldS18.spritefont"));
		wstring text = L"<Hangle><Red>테스<br>트, 테스트2</Red>!@#$% </Hangle><English>Test. ^<Blue>&*</Blue>() End</English>";
		textArea->SetFont("TextArea", text, layout, fontFileList);

		m_panel->AddComponent(move(textArea), { 0.5f, 0.5f });
		EXPECT_TRUE(m_renderer->LoadComponents());

		CallMockRender(m_panel.get(), TestTextAreaRender);
		EXPECT_TRUE(WriteReadTest(m_panel));

		unique_ptr<UIComponent> clonePanel = m_panel->Clone();
		clonePanel->ProcessUpdate({}, nullptr);

		CallMockRender(clonePanel.get(), TestTextAreaRender);
		EXPECT_TRUE(WriteReadTest(clonePanel));
	}

	TEST_F(ToyTestFixture, TestRecursivePosition)
	{
		std::unique_ptr<Panel> panel2 = std::make_unique<Panel>();
		panel2->SetLayout({ { 20, 20 }, Origin::Center });
		Panel* ptrPanel = panel2.get();

		std::unique_ptr<Panel> panel1 = std::make_unique<Panel>();
		panel1->SetLayout({ { 400, 400 }, Origin::Center });

		panel1->AddComponent(move(panel2), { 0.1f, 0.1f });
		m_panel->AddComponent(move(panel1), { 0.5f, 0.5f });

		vector<UIComponent*> outList;
		m_panel->GetComponents({ 240, 140 }, outList);
		EXPECT_EQ(outList.size(), 3);

		ptrPanel->ChangeOrigin(Origin::LeftTop);

		outList.clear();
		m_panel->GetComponents({ 239, 140 }, outList);
		EXPECT_EQ(outList.size(), 2);
	}
}