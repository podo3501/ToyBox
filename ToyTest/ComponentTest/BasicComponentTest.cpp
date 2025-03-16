#include "pch.h"
#include "../ToyTestFixture.h"
#include "../IMockRenderer.h"
#include "../TestHelper.h"
#include "../Utility.h"
#include "../Toy/Window.h"
#include "../Toy/UserInterface/Component/ImageGrid1.h"
#include "../Toy/UserInterface/Component/ImageGrid3.h"
#include "../Toy/UserInterface/Component/ImageGrid9.h"
#include "../Toy/UserInterface/Component/Button.h"
#include "../Toy/UserInterface/Component/Panel.h"
#include "../Toy/UserInterface/Component/TextArea.h"
#include "../Toy/UserInterface/Component/ImageSwitcher.h"
#include "../Toy/UserInterface/Component/SampleComponent.h"
#include "../Toy/UserInterface/UIUtility.h"
#include "../Toy/Utility.h"

using testing::ElementsAre;

namespace UserInterfaceTest
{
	static void TestButton_ImageGrid1Render(size_t index, const RECT& dest, const RECT* source, TextureSourceBinder* sb)
	{
		TestRender(index, dest, source, {
			{ { 144, 104, 176, 136 }, Binder(sb, "ExitButton1_Pressed", 0)}
			});
	}

	TEST_F(BasicComponentTest, Button_ImageGrid1)
	{
		auto [button, buttonPtr] = GetPtrs(CreateSampleButton1({ {32, 32}, Origin::Center }));
		UIEx(m_panel).AttachComponent(move(button), { 160, 120 });
		EXPECT_TRUE(m_renderer->LoadComponent(m_panel.get()));

		MockMouseInput(144, 120, true);	//Pressed
		CallMockRender(TestButton_ImageGrid1Render, 1);

		EXPECT_TRUE(WriteReadTest(m_panel));
	}

	////////////////////////////////////////////////////////////////
	
	TEST_F(BasicComponentTest, ImageChanger_ImageGrid1)
	{
		auto [imgSwitcher, imgSwitcherPtr] = GetPtrs(CreateSampleImageSwitcher1({ {32, 32}, Origin::Center }, BehaviorMode::Normal));
		UIEx(m_panel).AttachComponent(move(imgSwitcher), { 160, 120 });
		EXPECT_TRUE(m_renderer->LoadComponent(m_panel.get()));

		MockMouseInput(144, 120, true);	//Pressed
		CallMockRender(TestButton_ImageGrid1Render, 1);

		EXPECT_TRUE(WriteReadTest(m_panel));
	}

	////////////////////////////////////////////////////////////////

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

	TEST_F(BasicComponentTest, Button_ImageGrid3_Horizontal)
	{
		auto [button, btnPtr] = GetPtrs(CreateSampleButton3(DirectionType::Horizontal, { {100, 48}, Origin::Center }));
		UIEx(m_panel).AttachComponent(move(button), { 160, 120 });
		EXPECT_TRUE(m_renderer->LoadComponent(m_panel.get()));

		MockMouseInput(110, 96);	//Hover
		CallMockRender(TestButton_ImageGrid3Render_H, 3);

		btnPtr->ChangeSize({ 150, 48 });
		MockMouseInput(0, 0);	//Normal
		CallMockRender(TestButton_ImageGrid3ChangeAreaRender_H, 3);
		EXPECT_TRUE(WriteReadTest(m_panel));

		CloneTest(m_panel.get(), TestButton_ImageGrid3ChangeAreaRender_H, 3);
	}

	////////////////////////////////////////////////////////////////

	TEST_F(BasicComponentTest, ImageChanger_ImageGrid3_H)
	{
		//map<InteractState, ImageSource> sources{
			//{ InteractState::Normal, { L"UI/SampleTexture/Sample_0.png", { {10, 82, 22, 48}, {32, 82, 4, 48}, {36, 82, 22, 48} } } },
			//{ InteractState::Hover, { L"UI/SampleTexture/Sample_0.png", { {62, 82, 22, 48}, {84, 82, 4, 48}, {88, 82, 22, 48} } } },
			//{ InteractState::Pressed, { L"UI/SampleTexture/Sample_0.png", { {114, 82, 22, 48}, {136, 82, 4, 48}, {140, 82, 22, 48} } } } };

		//auto [imgChanger, imgChangerPtr] = GetPtrs(CreateImageChanger({ {100, 48}, Origin::Center },
			//make_unique<ImageGrid3>(DirectionType::Horizontal), sources, BehaviorMode::Normal));

		auto [imgSwitcher, imgSwitcherPtr] = GetPtrs(CreateSampleImageSwitcher3({ {100, 48}, Origin::Center },
			DirectionType::Horizontal, BehaviorMode::Normal));
		UIEx(m_panel).AttachComponent(move(imgSwitcher), { 160, 120 });
		EXPECT_TRUE(m_renderer->LoadComponent(m_panel.get()));

		MockMouseInput(110, 96);	//Hover
		CallMockRender(TestButton_ImageGrid3Render_H, 3);

		imgSwitcherPtr->ChangeSize({ 150, 48 });
		MockMouseInput(0, 0);	//Normal
		CallMockRender(TestButton_ImageGrid3ChangeAreaRender_H, 3);
		EXPECT_TRUE(WriteReadTest(m_panel));

		CloneTest(m_panel.get(), TestButton_ImageGrid3ChangeAreaRender_H, 3);
	}

	////////////////////////////////////////////////////////////////

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

	TEST_F(BasicComponentTest, Button_ImageGrid3_Vertical)
	{
		auto [button, btnPtr] = GetPtrs(CreateSampleButton3(DirectionType::Vertical, {{48, 100}, Origin::Center}));
		UIEx(m_panel).AttachComponent(move(button), { 100, 100 });
		EXPECT_TRUE(m_renderer->LoadComponent(m_panel.get()));

		MockMouseInput(77, 51);	//Hover
		CallMockRender(TestButton_ImageGrid3Render_V, 3);

		btnPtr->ChangeSize({ 48, 150 });
		MockMouseInput(0, 0);	//Normal
		CallMockRender(TestButton_ImageGrid3ChangeAreaRender_V, 3);
		EXPECT_TRUE(WriteReadTest(m_panel));

		CloneTest(m_panel.get(), TestButton_ImageGrid3ChangeAreaRender_V, 3);
	}

	////////////////////////////////////////////////////////

	static void TestImageGrid1Render(size_t index, const RECT& dest, const RECT* source, TextureSourceBinder* sb)
	{
		TestRender(index, dest, source, {
			{ { 368, 268, 432, 332 }, Binder(sb, "BackImage1", 0)}
			});
	}

	TEST_F(BasicComponentTest, ImageGrid1)
	{
		auto [img1, img1Ptr] = GetPtrs(CreateImageGrid1({ {64, 64}, Origin::Center }, "BackImage1"));
		UIEx(m_panel).AttachComponent(move(img1), { 400, 300 });
		EXPECT_TRUE(LoadComponent(m_renderer.get(), m_sourceBinder.get(), m_panel.get()));
		CallMockRender(TestImageGrid1Render, 1);

		img1Ptr->SetStateFlag(StateFlag::X_SizeLocked, true);
		img1Ptr->SetStateFlag(StateFlag::Y_SizeLocked, false);
		img1Ptr->ChangeSize({ 128, 64 });
		CallMockRender(TestImageGrid1Render, 1);

		img1Ptr->SetStateFlag(StateFlag::X_SizeLocked, false);
		img1Ptr->SetStateFlag(StateFlag::Y_SizeLocked, true);
		img1Ptr->ChangeSize({ 64, 128 });
		CallMockRender(TestImageGrid1Render, 1);

		img1Ptr->SetStateFlag(StateFlag::X_SizeLocked, true);
		img1Ptr->SetStateFlag(StateFlag::Y_SizeLocked, true);
		img1Ptr->ChangeSize({ 128, 128 });
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

	TEST_F(BasicComponentTest, ImageGrid3_Horizontal)
	{
		auto [img, imgPtr] = GetPtrs(CreateSampleImageGrid3(DirectionType::Horizontal, { {100, 36}, Origin::LeftTop }));
		//auto [img, imgPtr] = GetPtrs(CreateImageGrid3(DirectionType::Horizontal, { {100, 36}, Origin::LeftTop }));
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

	TEST_F(BasicComponentTest, ImageGrid3_Vertical)
	{
		auto [img3, img3Ptr] = GetPtrs(CreateSampleImageGrid3(DirectionType::Vertical, { {36, 100}, Origin::LeftTop }));
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

	TEST_F(BasicComponentTest, ImageGrid9)
	{
		auto [img, imgPtr] = GetPtrs(CreateSampleImageGrid9({ {170, 120}, Origin::LeftTop }));
		UIEx(m_panel).AttachComponent(move(img), { 400, 300 });
		EXPECT_TRUE(m_renderer->LoadComponent(m_panel.get()));

		CallMockRender(TestImageGrid9Render, 9);

		imgPtr->ChangeOrigin(Origin::Center);
		imgPtr->ChangeSize({ 180, 150 });

		CallMockRender(TestImageGrid9ChangeAreaRender, 9);
		EXPECT_TRUE(WriteReadTest(m_panel));

		SourceDivider srcDivider{ imgPtr->GetSourceAnd4Divider() };
		EXPECT_TRUE((srcDivider.rect == Rectangle{ 10, 10, 64, 64 }));
		EXPECT_THAT(srcDivider.list, ElementsAre(30, 34, 36, 38));

		srcDivider.list.clear();
		srcDivider.list = { 20, 44, 26, 48 };
		imgPtr->SetSourceAnd4Divider(srcDivider);

		CallMockRender(TestImageGrid9SourceAndDivider, 9);
	}

	////////////////////////////////////////////////////////

	static void TestTextAreaRender(size_t index, const wstring& text, const Vector2& pos, const FXMVECTOR& color)
	{
		if (text == L"테스") EXPECT_TRUE(index == 1 && pos == Vector2(240.f, 240.f) && DirectX::XMVector4Equal(color, Colors::Red));
		if (text == L"테스트2") EXPECT_TRUE(index == 1 && pos == Vector2(282, 268.375f) && DirectX::XMVector4Equal(color, Colors::Red));
		if (text == L"^") EXPECT_TRUE(index == 0 && pos == Vector2(531.f, 268.375f) && DirectX::XMVector4Equal(color, Colors::Black));
		if (text == L"&*") EXPECT_TRUE(index == 0 && pos == Vector2(240.f, 296.75f) && DirectX::XMVector4Equal(color, Colors::Blue));
	}

	TEST_F(BasicComponentTest, TextArea)
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

