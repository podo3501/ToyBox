#include "pch.h"
#include "ToyTestFixture.h"
#include "IMockRenderer.h"
#include "TestHelper.h"
#include "Utility.h"
#include "../Toy/Window.h"
#include "../Toy/UserInterface/Component/ImageGrid1.h"
#include "../Toy/UserInterface/Component/ImageGrid3.h"
#include "../Toy/UserInterface/Component/ImageGrid9.h"
#include "../Toy/UserInterface/Component/Button.h"
#include "../Toy/UserInterface/Component/Panel.h"
#include "../Toy/UserInterface/Component/ListArea.h"
#include "../Toy/UserInterface/Component/TextArea.h"
#include "../Toy/UserInterface/Component/DrawTexture.h"
#include "../Toy/UserInterface/Component/SampleComponent.h"
#include "../Toy/Utility.h"

using testing::ElementsAre;

namespace ComponentTest
{
	static void CloneTest(UIComponent* component, function<void(size_t, const RECT&, const RECT*)> renderFunc, int times)
	{
		unique_ptr<UIComponent> clonePanel = component->Clone();

		CallMockRender(clonePanel.get(), renderFunc, times);
		EXPECT_TRUE(WriteReadTest(clonePanel));
	}

	static void TestButton_ImageGrid1Render(size_t index, const RECT& dest, const RECT* source)
	{
		EXPECT_TRUE(index == 0);

		auto testResult{ false };
		testResult |= IsTrue(dest, { 144, 104, 176, 136 }, *source, { 10, 138, 42, 170 });
		testResult |= IsTrue(dest, { 144, 104, 176, 136 }, *source, { 46, 138, 78, 170 });
		testResult |= IsTrue(dest, { 144, 104, 176, 136 }, *source, { 82, 138, 114, 170 });

		EXPECT_TRUE(testResult);
	}

	TEST_F(BasicFunctionalityTest, Button_ImageGrid1)
	{
		auto button = CreateSampleButton1({ {32, 32}, Origin::Center });
		UIEx(m_panel).AttachComponent(move(button), { 160, 120 });
		EXPECT_TRUE(m_renderer->LoadComponent(m_panel.get()));

		TestUpdate(m_panel.get(), 144, 120);	//hover
		CallMockRender(m_panel.get(), TestButton_ImageGrid1Render, 1);
		EXPECT_TRUE(WriteReadTest(m_panel));
	}

	static void TestButton_ImageGrid3Render(size_t index, const RECT& dest, const RECT* source)
	{
		EXPECT_TRUE(index == 0);

		auto testResult{ false };
		testResult |= IsTrue(dest, { 110, 96, 132, 144 }, *source, { 62, 82, 84, 130 });
		testResult |= IsTrue(dest, { 132, 96, 188, 144 }, *source, { 84, 82, 88, 130 });
		testResult |= IsTrue(dest, { 188, 96, 210, 144 }, *source, { 88, 82, 110, 130 });

		EXPECT_TRUE(testResult);
	}

	static void TestButton_ImageGrid3ChangeAreaRender(size_t index, const RECT& dest, const RECT* source)
	{
		EXPECT_TRUE(index == 0);

		auto testResult{ false };
		testResult |= IsTrue(dest, { 85, 96, 107, 144 }, *source, { 10, 82, 32, 130 });
		testResult |= IsTrue(dest, { 107, 96, 213, 144 }, *source, { 32, 82, 36, 130 });
		testResult |= IsTrue(dest, { 213, 96, 235, 144 }, *source, { 36, 82, 58, 130 });

		EXPECT_TRUE(testResult);
	}

	TEST_F(BasicFunctionalityTest, Button_ImageGrid3)
	{
		auto button = CreateSampleButton3({ {100, 48}, Origin::Center });
		UIEx(m_panel).AttachComponent(move(button), { 160, 120 });
		EXPECT_TRUE(m_renderer->LoadComponent(m_panel.get()));

		TestUpdate(m_panel.get(), 110, 96);	//Hover
		CallMockRender(m_panel.get(), TestButton_ImageGrid3Render, 3);

		Button* btn = UIEx(m_panel).GetComponent<Button*>("Button_0");
		btn->ChangeSize({ 150, 48 });
		TestUpdate(m_panel.get(), 0, 0);	//Normal

		CallMockRender(m_panel.get(), TestButton_ImageGrid3ChangeAreaRender, 3);
		EXPECT_TRUE(WriteReadTest(m_panel));

		CloneTest(m_panel.get(), TestButton_ImageGrid3ChangeAreaRender, 3);
	}

	TEST_F(BasicFunctionalityTest, DrawTexture)
	{
		auto img1 = CreateSampleImageGrid1({ {64, 64}, Origin::LeftTop });

		ImTextureID texID{};
		auto drawTex = make_unique<DrawTexture>();
		drawTex->CreateTexture(m_renderer->GetValue(), img1->GetSize(), img1.get(), &texID);
	}

	////////////////////////////////////////////////////////

	static void TestImageGrid1Render(size_t index, const RECT& dest, const RECT* source)
	{
		EXPECT_TRUE(index == 0);
		EXPECT_TRUE(IsTrue(dest, { 400, 300, 464, 364 }, *source, { 10, 10, 74, 74 }));
	}

	TEST_F(BasicFunctionalityTest, ImageGrid1)
	{
		auto img1 = CreateSampleImageGrid1({ {64, 64}, Origin::LeftTop });
		UIEx(m_panel).AttachComponent(move(img1), { 400, 300 });
		EXPECT_TRUE(m_renderer->LoadComponent(m_panel.get()));

		CallMockRender(m_panel.get(), TestImageGrid1Render, 1);
		EXPECT_TRUE(WriteReadTest(m_panel));
	}

	////////////////////////////////////////////////////////

	static void TestImageGrid3Render(size_t index, const RECT& dest, const RECT* source)
	{
		EXPECT_TRUE(index == 0);

		auto testResult{ false };
		testResult |= IsTrue(dest, { 400, 300, 422, 336 }, *source, { 10, 82, 32, 130 });
		testResult |= IsTrue(dest, { 422, 300, 478, 336 }, *source, { 32, 82, 36, 130 });
		testResult |= IsTrue(dest, { 478, 300, 500, 336 }, *source, { 36, 82, 58, 130 });

		EXPECT_TRUE(testResult);
	}

	static void TestImageGrid3ChangeAreaRender(size_t index, const RECT& dest, const RECT* source)
	{
		EXPECT_TRUE(index == 0);

		auto testResult{ false };
		testResult |= IsTrue(dest, { 340, 282, 362, 318 }, *source, { 10, 82, 32, 130 });
		testResult |= IsTrue(dest, { 362, 282, 438, 318 }, *source, { 32, 82, 36, 130 });
		testResult |= IsTrue(dest, { 438, 282, 460, 318 }, *source, { 36, 82, 58, 130 });

		EXPECT_TRUE(testResult);
	}

	static void TestImageGrid3SourceAndDivider(size_t index, const RECT& dest, const RECT* source)
	{
		EXPECT_TRUE(index == 0);

		auto testResult{ false };
		testResult |= IsTrue(dest, { 340, 282, 362, 318 }, *source, { 10, 82, 30, 130 });
		testResult |= IsTrue(dest, { 362, 282, 438, 318 }, *source, { 30, 82, 38, 130 });
		testResult |= IsTrue(dest, { 438, 282, 460, 318 }, *source, { 38, 82, 58, 130 });

		EXPECT_TRUE(testResult);
	}

	TEST_F(BasicFunctionalityTest, ImageGrid3)
	{
		auto img = CreateSampleImageGrid3({ {100, 36}, Origin::LeftTop });
		UIEx(m_panel).AttachComponent(move(img), {400, 300});
		EXPECT_TRUE(m_renderer->LoadComponent(m_panel.get()));

		CallMockRender(m_panel.get(), TestImageGrid3Render, 3);

		ImageGrid3* img3 = UIEx(m_panel).GetComponent<ImageGrid3*>("ImageGrid3_0");
		img3->ChangeOrigin(Origin::Center);
		img3->ChangeSize({ 120, 36 });

		CallMockRender(m_panel.get(), TestImageGrid3ChangeAreaRender, 3);
		EXPECT_TRUE(WriteReadTest(m_panel));

		SourceDivider srcDivider{ *img3->GetSourceAnd2Divider() };
		EXPECT_TRUE((srcDivider.rect == Rectangle{ 10, 82, 48, 48 }));
		EXPECT_THAT(srcDivider.list, ElementsAre(22, 26));

		srcDivider.list.clear();
		srcDivider.list = { 20, 28 };
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

	TEST_F(BasicFunctionalityTest, ImageGrid9)
	{
		auto img = CreateSampleImageGrid9({ {170, 120}, Origin::LeftTop });
		UIEx(m_panel).AttachComponent(move(img), { 400, 300 });
		EXPECT_TRUE(m_renderer->LoadComponent(m_panel.get()));

		CallMockRender(m_panel.get(), TestImageGrid9Render, 9);

		ImageGrid9* img9 = UIEx(m_panel).GetComponent<ImageGrid9*>("ImageGrid9_0");
		img9->ChangeOrigin(Origin::Center);
		img9->ChangeSize({ 180, 150 });

		CallMockRender(m_panel.get(), TestImageGrid9ChangeAreaRender, 9);
		EXPECT_TRUE(WriteReadTest(m_panel));

		SourceDivider srcDivider{ *img9->GetSourceAnd4Divider() };
		EXPECT_TRUE((srcDivider.rect == Rectangle{ 10, 10, 64, 64 }));
		EXPECT_THAT(srcDivider.list, ElementsAre(30, 34, 36, 38));

		srcDivider.list.clear();
		srcDivider.list = { 20, 44, 26, 48 };
		img9->SetSourceAnd4Divider(srcDivider);

		CallMockRender(m_panel.get(), TestImageGrid9SourceAndDivider, 9);
	}

	////////////////////////////////////////////////////////

	TEST_F(BasicFunctionalityTest, ListArea)
	{
		auto listArea = CreateSampleListArea1({ { 150, 130 }, Origin::Center });
		auto listAreaPtr = ComponentCast<ListArea*>(listArea.get());
		UIEx(m_panel).AttachComponent(move(listArea), { 400, 300 });
		EXPECT_TRUE(m_renderer->LoadComponent(m_panel.get()));

		EXPECT_TRUE(MakeSampleListAreaData(m_renderer.get(), listAreaPtr));
	}

	////////////////////////////////////////////////////////

	static void TestTextAreaRender(size_t index, const wstring& text, const Vector2& pos, const FXMVECTOR& color)
	{
		if (text == L"테스") EXPECT_TRUE(index == 1 && pos == Vector2(240.f, 240.f) && DirectX::XMVector4Equal(color, Colors::Red));
		if (text == L"테스트2") EXPECT_TRUE(index == 1 && pos == Vector2(282, 268.375f) && DirectX::XMVector4Equal(color, Colors::Red));
		if (text == L"^") EXPECT_TRUE(index == 0 && pos == Vector2(531.f, 268.375f) && DirectX::XMVector4Equal(color, Colors::Black));
		if (text == L"&*") EXPECT_TRUE(index == 0 && pos == Vector2(240.f, 296.75f) && DirectX::XMVector4Equal(color, Colors::Blue));
	}

	TEST_F(BasicFunctionalityTest, TextArea)
	{
		wstring text = L"<Hangle><Red>테스<br>트, 테스트2</Red>!@#$% </Hangle><English>Test. ^<Blue>&*</Blue>() End</English>";
		auto textArea = CreateSampleTextArea({ {320, 120}, Origin::Center }, text);
		UIEx(m_panel).AttachComponent(move(textArea), { 400, 300 });
		EXPECT_TRUE(m_renderer->LoadComponent(m_panel.get()));

		CallMockRender(m_panel.get(), TestTextAreaRender);
		EXPECT_TRUE(WriteReadTest(m_panel));

		unique_ptr<UIComponent> clonePanel = m_panel->Clone();
		CallMockRender(clonePanel.get(), TestTextAreaRender);
		EXPECT_TRUE(WriteReadTest(clonePanel));
	}
}

