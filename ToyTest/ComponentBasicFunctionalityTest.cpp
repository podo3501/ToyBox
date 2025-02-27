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
#include "../Toy/UserInterface/Component/RenderTexture.h"
#include "../Toy/UserInterface/Component/SampleComponent.h"
#include "../Toy/UserInterface/Component/ScrollBar.h"
#include "../Toy/UserInterface/UIUtility.h"
#include "../Toy/Utility.h"

using testing::ElementsAre;

namespace ComponentTest
{
	static void TestButton_ImageGrid1Render(size_t index, const RECT& dest, const RECT* source)
	{
		TestRender(index, dest, source, {
			{{144, 104, 176, 136}, {10, 138, 42, 170}}, //Normal
			{{144, 104, 176, 136}, {46, 138, 78, 170}}, //hovered
			{{144, 104, 176, 136}, {82, 138, 114, 170}} //Pressed
			});
	}

	TEST_F(BasicFunctionalityTest, Button_ImageGrid1)
	{
		auto button = CreateSampleButton1({ {32, 32}, Origin::Center });
		auto buttonPtr = button.get();
		UIEx(m_panel).AttachComponent(move(button), { 160, 120 });
		EXPECT_TRUE(m_renderer->LoadComponent(m_panel.get()));

		TestUpdate(144, 120, true);	//Pressed
		CallMockRender(TestButton_ImageGrid1Render, 1);

		EXPECT_TRUE(WriteReadTest(m_panel));
	}

	static void TestButton_ImageGrid3Render_H(size_t index, const RECT& dest, const RECT* source)
	{
		TestRender(index, dest, source, {
			{{110, 96, 132, 144}, {62, 82, 84, 130}},
			{{132, 96, 188, 144}, {84, 82, 88, 130}},
			{{188, 96, 210, 144}, {88, 82, 110, 130}}
			});
	}

	static void TestButton_ImageGrid3ChangeAreaRender_H(size_t index, const RECT& dest, const RECT* source)
	{
		TestRender(index, dest, source, {
			{{85, 96, 107, 144}, {10, 82, 32, 130}},
			{{107, 96, 213, 144}, {32, 82, 36, 130}},
			{{213, 96, 235, 144}, {36, 82, 58, 130}}
			});
	}

	TEST_F(BasicFunctionalityTest, Button_ImageGrid3_Horizontal)
	{
		auto button = CreateSampleButton3(DirectionType::Horizontal, { {100, 48}, Origin::Center });
		auto btnPtr = ComponentCast<Button*>(button.get());
		UIEx(m_panel).AttachComponent(move(button), { 160, 120 });
		EXPECT_TRUE(m_renderer->LoadComponent(m_panel.get()));

		TestUpdate(110, 96);	//Hover
		CallMockRender(TestButton_ImageGrid3Render_H, 3);

		btnPtr->ChangeSize({ 150, 48 });
		TestUpdate(0, 0);	//Normal
		CallMockRender(TestButton_ImageGrid3ChangeAreaRender_H, 3);
		EXPECT_TRUE(WriteReadTest(m_panel));

		CloneTest(m_panel.get(), TestButton_ImageGrid3ChangeAreaRender_H, 3);
	}

	static void TestButton_ImageGrid3Render_V(size_t index, const RECT& dest, const RECT* source)
	{
		TestRender(index, dest, source, {
			{{76, 50, 124, 72}, {62, 82, 110, 104}},
			{{76, 72, 124, 128}, {62, 104, 110, 108}},
			{{76, 128, 124, 150}, {62, 108, 110, 130}}
			});
	}

	static void TestButton_ImageGrid3ChangeAreaRender_V(size_t index, const RECT& dest, const RECT* source)
	{
		TestRender(index, dest, source, {
			{{76, 25, 124, 47}, {10, 82, 58, 104}},
			{{76, 47, 124, 153}, {10, 104, 58, 108}},
			{{76, 153, 124, 175}, {10, 108, 58, 130}}
			});
	}

	TEST_F(BasicFunctionalityTest, Button_ImageGrid3_Vertical)
	{
		auto button = CreateSampleButton3(DirectionType::Vertical, { {48, 100}, Origin::Center });
		auto btnPtr = ComponentCast<Button*>(button.get());
		UIEx(m_panel).AttachComponent(move(button), { 100, 100 });
		EXPECT_TRUE(m_renderer->LoadComponent(m_panel.get()));

		TestUpdate(77, 51);	//Hover
		CallMockRender(TestButton_ImageGrid3Render_V, 3);

		btnPtr->ChangeSize({ 48, 150 });
		TestUpdate(0, 0);	//Normal
		CallMockRender(TestButton_ImageGrid3ChangeAreaRender_V, 3);
		EXPECT_TRUE(WriteReadTest(m_panel));

		CloneTest(m_panel.get(), TestButton_ImageGrid3ChangeAreaRender_V, 3);
	}

	////////////////////////////////////////////////////////

	static void TestImageGrid1Render(size_t index, const RECT& dest, const RECT* source)
	{
		TestRender(index, dest, source, {
			{{400, 300, 464, 364}, {10, 10, 74, 74}}
			});
	}

	TEST_F(BasicFunctionalityTest, ImageGrid1)
	{
		auto img1 = CreateSampleImageGrid1({ {64, 64}, Origin::LeftTop });
		UIEx(m_panel).AttachComponent(move(img1), { 400, 300 });
		EXPECT_TRUE(m_renderer->LoadComponent(m_panel.get()));

		CallMockRender(TestImageGrid1Render, 1);
		EXPECT_TRUE(WriteReadTest(m_panel));
	}

	////////////////////////////////////////////////////////

	static void TestImageGrid3Render_H(size_t index, const RECT& dest, const RECT* source)
	{
		TestRender(index, dest, source, {
			{{400, 300, 422, 336}, {10, 82, 32, 130}},
			{{422, 300, 478, 336}, {32, 82, 36, 130}},
			{{478, 300, 500, 336}, {36, 82, 58, 130}}
			});
	}

	static void TestImageGrid3ChangeAreaRender_H(size_t index, const RECT& dest, const RECT* source)
	{
		TestRender(index, dest, source, {
			{{340, 282, 362, 318}, {10, 82, 32, 130}},
			{{362, 282, 438, 318}, {32, 82, 36, 130}},
			{{438, 282, 460, 318}, {36, 82, 58, 130}}
			});
	}

	static void TestImageGrid3SourceAndDivider_H(size_t index, const RECT& dest, const RECT* source)
	{
		TestRender(index, dest, source, {
			{{340, 282, 362, 318}, {10, 82, 30, 130}},
			{{362, 282, 438, 318}, {30, 82, 38, 130}},
			{{438, 282, 460, 318}, {38, 82, 58, 130}}
			});
	}

	TEST_F(BasicFunctionalityTest, ImageGrid3_Horizontal)
	{
		auto img = CreateSampleImageGrid3(DirectionType::Horizontal, { {100, 36}, Origin::LeftTop });
		auto imgPtr = ComponentCast<ImageGrid3*>(img.get());
		UIEx(m_panel).AttachComponent(move(img), {400, 300});
		EXPECT_TRUE(m_renderer->LoadComponent(m_panel.get()));

		CallMockRender(TestImageGrid3Render_H, 3);

		imgPtr->ChangeOrigin(Origin::Center);
		imgPtr->ChangeSize({ 120, 36 });

		CallMockRender(TestImageGrid3ChangeAreaRender_H, 3);
		EXPECT_TRUE(WriteReadTest(m_panel));

		SourceDivider srcDivider{ imgPtr->GetSourceAnd2Divider() };
		EXPECT_TRUE((srcDivider.rect == Rectangle{ 10, 82, 48, 48 }));
		EXPECT_THAT(srcDivider.list, ElementsAre(22, 26));

		srcDivider.list.clear();
		srcDivider.list = { 20, 28 };
		imgPtr->SetSourceAnd2Divider(srcDivider);

		CallMockRender(TestImageGrid3SourceAndDivider_H, 3);
	}

	static void TestImageGrid3Render_V(size_t index, const RECT& dest, const RECT* source)
	{
		TestRender(index, dest, source, {
			{{400, 300, 436, 322}, {10, 82, 58, 104}},
			{{400, 322, 436, 378}, {10, 104, 58, 108}},
			{{400, 378, 436, 400}, {10, 108, 58, 130}}
			});
	}

	static void TestImageGrid3ChangeAreaRender_V(size_t index, const RECT& dest, const RECT* source)
	{
		TestRender(index, dest, source, {
			{{382, 240, 418, 262}, {10, 82, 58, 104}},
			{{382, 262, 418, 338}, {10, 104, 58, 108}},
			{{382, 338, 418, 360}, {10, 108, 58, 130}}
			});
	}

	static void TestImageGrid3SourceAndDivider_V(size_t index, const RECT& dest, const RECT* source)
	{
		TestRender(index, dest, source, {
			{{382, 240, 418, 262}, {10, 82, 58, 102}},
			{{382, 262, 418, 338}, {10, 102, 58, 110}},
			{{382, 338, 418, 360}, {10, 110, 58, 130}}
			});
	}

	TEST_F(BasicFunctionalityTest, ImageGrid3_Vertical)
	{
		auto img3 = CreateSampleImageGrid3(DirectionType::Vertical, { {36, 100}, Origin::LeftTop });
		auto img3Ptr = ComponentCast<ImageGrid3*>(img3.get());
		UIEx(m_panel).AttachComponent(move(img3), { 400, 300 });
		EXPECT_TRUE(m_renderer->LoadComponent(m_panel.get()));

		CallMockRender(TestImageGrid3Render_V, 3);

		img3Ptr->ChangeOrigin(Origin::Center);
		img3Ptr->ChangeSize({ 36, 120 });
		CallMockRender(TestImageGrid3ChangeAreaRender_V, 3);
		EXPECT_TRUE(WriteReadTest(m_panel));

		SourceDivider srcDivider{ img3Ptr->GetSourceAnd2Divider() };
		EXPECT_TRUE((srcDivider.rect == Rectangle{ 10, 82, 48, 48 }));
		EXPECT_THAT(srcDivider.list, ElementsAre(22, 26));

		srcDivider.list.clear();
		srcDivider.list = { 20, 28 };
		img3Ptr->SetSourceAnd2Divider(srcDivider);

		CallMockRender(TestImageGrid3SourceAndDivider_V, 3);
	}

	////////////////////////////////////////////////////////

	static void TestImageGrid9Render(size_t index, const RECT& dest, const RECT* source)
	{
		TestRender(index, dest, source, {
			{{400, 300, 430, 336}, {10, 10, 40, 46}},
			{{430, 300, 540, 336}, {40, 10, 44, 46}},
			{{540, 300, 570, 336}, {44, 10, 74, 46}},

			{{400, 336, 430, 394}, {10, 46, 40, 48}},
			{{430, 336, 540, 394}, {40, 46, 44, 48}},
			{{540, 336, 570, 394}, {44, 46, 74, 48}},

			{{400, 394, 430, 420}, {10, 48, 40, 74}},
			{{430, 394, 540, 420}, {40, 48, 44, 74}},
			{{540, 394, 570, 420}, {44, 48, 74, 74}}
			});
	}

	static void TestImageGrid9ChangeAreaRender(size_t index, const RECT& dest, const RECT* source)
	{
		TestRender(index, dest, source, {
			{{310, 225, 340, 261}, {10, 10, 40, 46}},
			{{340, 225, 460, 261}, {40, 10, 44, 46}},
			{{460, 225, 490, 261}, {44, 10, 74, 46}},

			{{310, 261, 340, 349}, {10, 46, 40, 48}},
			{{340, 261, 460, 349}, {40, 46, 44, 48}},
			{{460, 261, 490, 349}, {44, 46, 74, 48}},

			{{310, 349, 340, 375}, {10, 48, 40, 74}},
			{{340, 349, 460, 375}, {40, 48, 44, 74}},
			{{460, 349, 490, 375}, {44, 48, 74, 74}}
			});
	}

	static void TestImageGrid9SourceAndDivider(size_t index, const RECT& dest, const RECT* source)
	{
		TestRender(index, dest, source, {
			{{310, 225, 340, 261}, {10, 10, 30, 36}},
			{{340, 225, 460, 261}, {30, 10, 54, 36}},
			{{460, 225, 490, 261}, {54, 10, 74, 36}},

			{{310, 261, 340, 349}, {10, 36, 30, 58}},
			{{340, 261, 460, 349}, {30, 36, 54, 58}},
			{{460, 261, 490, 349}, {54, 36, 74, 58}},

			{{310, 349, 340, 375}, {10, 58, 30, 74}},
			{{340, 349, 460, 375}, {30, 58, 54, 74}},
			{{460, 349, 490, 375}, {54, 58, 74, 74}}
			});
	}

	TEST_F(BasicFunctionalityTest, ImageGrid9)
	{
		auto img = CreateSampleImageGrid9({ {170, 120}, Origin::LeftTop });
		UIEx(m_panel).AttachComponent(move(img), { 400, 300 });
		EXPECT_TRUE(m_renderer->LoadComponent(m_panel.get()));

		CallMockRender(TestImageGrid9Render, 9);

		ImageGrid9* img9 = UIEx(m_panel).GetComponent<ImageGrid9*>("ImageGrid9_0");
		img9->ChangeOrigin(Origin::Center);
		img9->ChangeSize({ 180, 150 });

		CallMockRender(TestImageGrid9ChangeAreaRender, 9);
		EXPECT_TRUE(WriteReadTest(m_panel));

		SourceDivider srcDivider{ img9->GetSourceAnd4Divider() };
		EXPECT_TRUE((srcDivider.rect == Rectangle{ 10, 10, 64, 64 }));
		EXPECT_THAT(srcDivider.list, ElementsAre(30, 34, 36, 38));

		srcDivider.list.clear();
		srcDivider.list = { 20, 44, 26, 48 };
		img9->SetSourceAnd4Divider(srcDivider);

		CallMockRender(TestImageGrid9SourceAndDivider, 9);
	}

	////////////////////////////////////////////////////////

	TEST_F(BasicFunctionalityTest, ListArea)
	{
		auto listArea = CreateSampleListArea1({ { 150, 130 }, Origin::Center });
		auto listAreaPtr = ComponentCast<ListArea*>(listArea.get());
		UIEx(m_panel).AttachComponent(move(listArea), { 400, 300 });
		EXPECT_TRUE(m_renderer->LoadComponent(m_panel.get()));

		EXPECT_TRUE(MakeSampleListAreaData(m_renderer.get(), listAreaPtr));
		EXPECT_TRUE(WriteReadTest(m_panel));
	}

	////////////////////////////////////////////////////////

	static void TestRenderTexture(size_t index, const RECT& dest, const RECT* source)
	{
		EXPECT_EQ(index, 1); //0은 texture 1은 renderTexture이다. 그래서 1이 들어오고
		TestRender(0, dest, source, {//0값 비교하니까 0을 그냥 넣어줌. 
			{{75, 75, 125, 125}, {0, 0, 50, 50}}
			});
	}

	TEST_F(BasicFunctionalityTest, RenderTexture)
	{
		auto button = CreateSampleButton1({ {32, 32}, Origin::LeftTop });
		auto buttonPtr = button.get();
		auto renderTex = CreateRenderTexture({ { 50, 50 }, Origin::Center }, move(button));
		UIEx(m_panel).AttachComponent(move(renderTex), { 100, 100 });
		EXPECT_TRUE(m_renderer->LoadComponent(m_panel.get()));

		CallMockRender(TestRenderTexture, 1); //core에 렌더코드가 안 돌기 때문에 한번만 들어온다.
		EXPECT_TRUE(WriteReadTest(m_panel));
	}

	////////////////////////////////////////////////////////

	static void TestScrollBar(size_t index, const RECT& dest, const RECT* source)
	{
		TestRender(index, dest, source, {
			{{75, 75, 125, 125}, {0, 0, 50, 50}}
			});
	}


	TEST_F(BasicFunctionalityTest, ScrollBar)
	{
		auto scrollBar = CreateSampleScrollBar(DirectionType::Vertical, { { 16, 200 }, Origin::Center });
		auto scrollBarPtr = scrollBar.get();
		UIEx(m_panel).AttachComponent(move(scrollBar), { 100, 200 });
		EXPECT_TRUE(m_renderer->LoadComponent(m_panel.get()));

		uint32_t viewArea = 500;
		uint32_t contentSize = 2000;
		scrollBarPtr->SetViewContentRatio(static_cast<float>(viewArea) / static_cast<float>(contentSize));
		scrollBarPtr->SetPositionRatio(0.5f);

		//CallMockRender(TestScrollBar, 6);
		EXPECT_TRUE(WriteReadTest(m_panel));
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

		CallMockRender(TestTextAreaRender);
		EXPECT_TRUE(WriteReadTest(m_panel));

		unique_ptr<UIComponent> clonePanel = m_panel->Clone();
		CallMockRender(clonePanel.get(), TestTextAreaRender);
		EXPECT_TRUE(WriteReadTest(clonePanel));
	}
}

