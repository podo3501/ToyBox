#include "pch.h"
#include "../ToyTestFixture.h"
#include "../IMockRenderer.h"
#include "../TestHelper.h"
#include "../Toy/UserInterface/UIComponent/Components/ImageGrid1.h"
#include "../Toy/UserInterface/UIComponent/Components/ImageGrid3.h"
#include "../Toy/UserInterface/UIComponent/Components/ImageGrid9.h"
#include "../Toy/UserInterface/UIComponent/Components/Container.h"
#include "../Toy/UserInterface/UIComponent/Components/TextArea.h"
#include "../Toy/UserInterface/UIComponent/Components/ImageSwitcher.h"
#include "../Toy/UserInterface/UIComponent/Components/SampleComponent.h"
#include "../Toy/UserInterface/TextureSourceBinder/TextureSourceBinder.h"

using testing::ElementsAre;

namespace UserInterfaceTest
{
	static void TestButton_ImageGrid1Render(size_t index, const RECT& dest, const RECT* source, TextureSourceBinder* sb)
	{
		vector<RECT> expectDest = { { 144, 104, 176, 136 } };
		TestCoordinates(index, dest, source, expectDest, GetSources(sb, "ExitButton1_Pressed"));
	}

	TEST_F(BasicComponentTest, Container_ImageGrid1)
	{
		UILayout layout{ {32, 32}, Origin::Center };
		auto container = CreateContainer(layout, GetComponentKeyMap(layout.GetSize(), "ExitButton1"), BehaviorMode::Normal);
		UIEx(m_panel).AttachComponent(move(container), { 160, 120 });
		WriteReadTest(m_panel);
		EXPECT_TRUE(m_panel->BindTextureSourceInfo(m_sourceBinder.get(), nullptr));

		MockMouseInput(144, 120, true);	//Pressed
		CallMockRender(TestButton_ImageGrid1Render, 1);
	}

	////////////////////////////////////////////////////////////////

	TEST_F(BasicComponentTest, ImageSwitcher_ImageGrid1)
	{
		auto switcher = CreateImageSwitcher({ {32, 32}, Origin::Center }, ImagePart::One, GetStateKeyMap("ExitButton1"), BehaviorMode::Normal);
		UIEx(m_panel).AttachComponent(move(switcher), { 160, 120 });
		m_panel = WriteReadTest(m_panel);
		EXPECT_TRUE(m_panel->BindTextureSourceInfo(m_sourceBinder.get(), nullptr));

		MockMouseInput(144, 120, true);	//Pressed
		CallMockRender(TestButton_ImageGrid1Render, 1);
	}

	////////////////////////////////////////////////////////////////

	static void TestButton_ImageGrid3Render_H(size_t index, const RECT& dest, const RECT* source, TextureSourceBinder* sb)
	{
		vector<RECT> expectDest = { { 110, 96, 132, 144 }, { 132, 96, 188, 144 }, {188, 96, 210, 144} };
		TestCoordinates(index, dest, source, expectDest, GetSources(sb, "ScrollButton3_H_Hovered"));
	}

	static void TestButton_ImageGrid3ChangeAreaRender_H(size_t index, const RECT& dest, const RECT* source, TextureSourceBinder* sb)
	{
		vector<RECT> expectDest = { { 85, 96, 107, 144 }, { 107, 96, 213, 144 }, {213, 96, 235, 144} };
		TestCoordinates(index, dest, source, expectDest, GetSources(sb, "ScrollButton3_H_Normal"));
	}

	TEST_F(BasicComponentTest, Container_ImageGrid3_Horizontal)
	{
		UILayout layout{ {100, 48}, Origin::Center };
		auto [container, containerBtr] = GetPtrs(CreateContainer(layout, 
			GetComponentKeyMap(DirectionType::Horizontal, layout.GetSize(), "ScrollButton3_H"), BehaviorMode::Normal));
		UIEx(m_panel).AttachComponent(move(container), { 160, 120 });
		m_panel = WriteReadTest(m_panel, containerBtr);
		EXPECT_TRUE(m_panel->BindTextureSourceInfo(m_sourceBinder.get(), nullptr));

		MockMouseInput(110, 96);	//Hover
		CallMockRender(TestButton_ImageGrid3Render_H, 3);

		containerBtr->ChangeSize({ 150, 48 });
		MockMouseInput(0, 0);	//Normal
		CallMockRender(TestButton_ImageGrid3ChangeAreaRender_H, 3);

		CloneTest(m_panel.get(), TestButton_ImageGrid3ChangeAreaRender_H, 3);
	}

	////////////////////////////////////////////////////////////////

	TEST_F(BasicComponentTest, ImageSwitcher_ImageGrid3_H)
	{
		auto [switcher, switcherPtr] = GetPtrs(CreateImageSwitcher({{100, 48}, Origin::Center},
			ImagePart::ThreeH, GetStateKeyMap("ScrollButton3_H"), BehaviorMode::Normal));
		UIEx(m_panel).AttachComponent(move(switcher), { 160, 120 });
		m_panel = WriteReadTest(m_panel, switcherPtr);
		EXPECT_TRUE(m_panel->BindTextureSourceInfo(m_sourceBinder.get(), nullptr));

		MockMouseInput(110, 96);	//Hover
		CallMockRender(TestButton_ImageGrid3Render_H, 3);

		switcherPtr->ChangeSize({ 150, 48 });
		MockMouseInput(0, 0);	//Normal
		CallMockRender(TestButton_ImageGrid3ChangeAreaRender_H, 3);

		CloneTest(m_panel.get(), TestButton_ImageGrid3ChangeAreaRender_H, 3);
	}

	////////////////////////////////////////////////////////////////

	static void TestButton_ImageGrid3Render_V(size_t index, const RECT& dest, const RECT* source, TextureSourceBinder* sb)
	{
		vector<RECT> expectDest = { { 76, 50, 124, 57 }, { 76, 57, 124, 143 }, {76, 143, 124, 150} };
		TestCoordinates(index, dest, source, expectDest, GetSources(sb, "ScrollButton3_V_Hovered"));
	}

	static void TestButton_ImageGrid3ChangeAreaRender_V(size_t index, const RECT& dest, const RECT* source, TextureSourceBinder* sb)
	{
		vector<RECT> expectDest = { { 76, 25, 124, 32 }, { 76, 32, 124, 168 }, {76, 168, 124, 175} };
		TestCoordinates(index, dest, source, expectDest, GetSources(sb, "ScrollButton3_V_Normal"));
	}

	TEST_F(BasicComponentTest, Container_ImageGrid3_Vertical)
	{
		UILayout layout{ {48, 100}, Origin::Center };
		auto [container, containerPtr] = GetPtrs(CreateContainer(layout, 
			GetComponentKeyMap(DirectionType::Vertical, layout.GetSize(), "ScrollButton3_V"), BehaviorMode::Normal));
		UIEx(m_panel).AttachComponent(move(container), { 100, 100 });
		m_panel = WriteReadTest(m_panel, containerPtr);
		EXPECT_TRUE(m_panel->BindTextureSourceInfo(m_sourceBinder.get(), nullptr));

		MockMouseInput(77, 51);	//Hover
		CallMockRender(TestButton_ImageGrid3Render_V, 3);

		containerPtr->ChangeSize({ 48, 150 });
		MockMouseInput(0, 0);	//Normal
		CallMockRender(TestButton_ImageGrid3ChangeAreaRender_V, 3);		

		CloneTest(m_panel.get(), TestButton_ImageGrid3ChangeAreaRender_V, 3);
	}

	////////////////////////////////////////////////////////////////

	TEST_F(BasicComponentTest, ImageSwitcher_ImageGrid3_V)
	{
		auto [switcher, switcherPtr] = GetPtrs(CreateImageSwitcher({ {48, 100}, Origin::Center },
			ImagePart::ThreeV, GetStateKeyMap("ScrollButton3_V"), BehaviorMode::Normal));
		UIEx(m_panel).AttachComponent(move(switcher), { 100, 100 });
		m_panel = WriteReadTest(m_panel, switcherPtr);
		EXPECT_TRUE(m_panel->BindTextureSourceInfo(m_sourceBinder.get(), nullptr));

		MockMouseInput(77, 51);	//Hover
		CallMockRender(TestButton_ImageGrid3Render_V, 3);

		switcherPtr->ChangeSize({ 48, 150 });
		MockMouseInput(0, 0);	//Normal
		CallMockRender(TestButton_ImageGrid3ChangeAreaRender_V, 3);

		//CloneTest(m_panel.get(), TestButton_ImageGrid3ChangeAreaRender_V, 3);
	}

	////////////////////////////////////////////////////////

	static void TestImageGrid1Render(size_t index, const RECT& dest, const RECT* source, TextureSourceBinder* sb)
	{
		vector<RECT> expectDest = { { 368, 268, 432, 332 } };
		TestCoordinates(index, dest, source, expectDest, GetSources(sb, "BackImage1"));
	}

	TEST_F(BasicComponentTest, ImageGrid1)
	{
		auto [img1, img1Ptr] = GetPtrs(CreateImageGrid1({ {64, 64}, Origin::Center }, "BackImage1"));
		UIEx(m_panel).AttachComponent(move(img1), { 400, 300 });
		m_panel = WriteReadTest(m_panel, img1Ptr);
		EXPECT_TRUE(m_panel->BindTextureSourceInfo(m_sourceBinder.get(), nullptr));
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
	}

	////////////////////////////////////////////////////////

	static void TestImageGrid3Render_H(size_t index, const RECT& dest, const RECT* source, TextureSourceBinder* sb)
	{
		vector<RECT> expectDest = { { 400, 300, 422, 336 }, { 422, 300, 478, 336 }, {478, 300, 500, 336} };
		TestCoordinates(index, dest, source, expectDest, GetSources(sb, "ScrollButton3_H_Normal"));
	}

	static void TestImageGrid3ChangeAreaRender_H(size_t index, const RECT& dest, const RECT* source, TextureSourceBinder* sb)
	{
		vector<RECT> expectDest = { { 340, 282, 362, 318 }, { 362, 282, 438, 318 }, {438, 282, 460, 318} };
		TestCoordinates(index, dest, source, expectDest, GetSources(sb, "ScrollButton3_H_Normal"));
	}

	TEST_F(BasicComponentTest, ImageGrid3_Horizontal)
	{
		auto [img, imgPtr] = GetPtrs(CreateImageGrid3(DirectionType::Horizontal, { {100, 36}, Origin::LeftTop }, "ScrollButton3_H_Normal"));
		UIEx(m_panel).AttachComponent(move(img), {400, 300});
		m_panel = WriteReadTest(m_panel, imgPtr);
		EXPECT_TRUE(m_panel->BindTextureSourceInfo(m_sourceBinder.get(), nullptr));

		CallMockRender(TestImageGrid3Render_H, 3);

		imgPtr->ChangeOrigin(Origin::Center);
		imgPtr->ChangeSize({ 120, 36 });

		CallMockRender(TestImageGrid3ChangeAreaRender_H, 3);

		SourceDivider srcDivider = GetSourceDivider(m_sourceBinder.get(), "ScrollButton3_H_Normal");
		EXPECT_TRUE((srcDivider.rect == Rectangle{ 10, 82, 48, 48 }));
		EXPECT_THAT(srcDivider.list, ElementsAre(22, 26));

		srcDivider.list.clear();
		srcDivider.list = { 20, 28 };
		EXPECT_TRUE(ModifyTextureSourceInfo(m_sourceBinder.get(), "ScrollButton3_H_Normal", srcDivider));

		m_panel = WriteReadTest(m_panel);
		EXPECT_TRUE(m_panel->BindTextureSourceInfo(m_sourceBinder.get(), nullptr));

		CallMockRender(TestImageGrid3ChangeAreaRender_H, 3);
	}

	////////////////////////////////////////////////////////

	static void TestImageGrid3Render_V(size_t index, const RECT& dest, const RECT* source, TextureSourceBinder* sb)
	{
		vector<RECT> expectDest = { { 400, 300, 436, 310 }, { 400, 310, 436, 390 }, { 400, 390, 436, 400 } };
		TestCoordinates(index, dest, source, expectDest, GetSources(sb, "ScrollTrack3_V"));
	}

	static void TestImageGrid3ChangeAreaRender_V(size_t index, const RECT& dest, const RECT* source, TextureSourceBinder* sb)
	{
		vector<RECT> expectDest = { { 382, 240, 418, 250 }, { 382, 250, 418, 350 }, { 382, 350, 418, 360 } };
		TestCoordinates(index, dest, source, expectDest, GetSources(sb, "ScrollTrack3_V"));
	}

	TEST_F(BasicComponentTest, ImageGrid3_Vertical)
	{
		auto [img3, img3Ptr] = GetPtrs(CreateImageGrid3(DirectionType::Vertical, { {36, 100}, Origin::LeftTop }, "ScrollTrack3_V"));
		UIEx(m_panel).AttachComponent(move(img3), { 400, 300 });
		m_panel = WriteReadTest(m_panel, img3Ptr);
		EXPECT_TRUE(m_panel->BindTextureSourceInfo(m_sourceBinder.get(), nullptr));

		CallMockRender(TestImageGrid3Render_V, 3);

		img3Ptr->ChangeOrigin(Origin::Center);
		img3Ptr->ChangeSize({ 36, 120 });
		CallMockRender(TestImageGrid3ChangeAreaRender_V, 3);

		SourceDivider srcDivider = GetSourceDivider(m_sourceBinder.get(), "ScrollTrack3_V");
		EXPECT_TRUE((srcDivider.rect == Rectangle{ 114, 178, 16, 48 }));
		EXPECT_THAT(srcDivider.list, ElementsAre(10, 38));

		srcDivider.list.clear();
		srcDivider.list = { 14, 34 };
		EXPECT_TRUE(ModifyTextureSourceInfo(m_sourceBinder.get(), "ScrollTrack3_V", srcDivider));

		m_panel = WriteReadTest(m_panel);
		EXPECT_TRUE(m_panel->BindTextureSourceInfo(m_sourceBinder.get(), nullptr));
		
		CallMockRender(TestImageGrid3ChangeAreaRender_V, 3);
	}

	////////////////////////////////////////////////////////

	static void TestImageGrid9Render(size_t index, const RECT& dest, const RECT* source, TextureSourceBinder* sb)
	{
		vector<RECT> expectDest = {
			{ 400, 300, 430, 336 }, { 430, 300, 540, 336 }, {540, 300, 570, 336},
			{ 400, 336, 430, 394 }, { 430, 336, 540, 394 }, {540, 336, 570, 394},
			{ 400, 394, 430, 420 }, { 430, 394, 540, 420 }, {540, 394, 570, 420} };
		TestCoordinates(index, dest, source, expectDest, GetSources(sb, "BackImage9"));
	}

	static void TestImageGrid9ChangeAreaRender(size_t index, const RECT& dest, const RECT* source, TextureSourceBinder* sb)
	{
		vector<RECT> expectDest = {
			{ 310, 225, 340, 261 }, { 340, 225, 460, 261 }, {460, 225, 490, 261},
			{ 310, 261, 340, 349 }, { 340, 261, 460, 349 }, {460, 261, 490, 349},
			{ 310, 349, 340, 375 }, { 340, 349, 460, 375 }, {460, 349, 490, 375} };
		TestCoordinates(index, dest, source, expectDest, GetSources(sb, "BackImage9"));
	}

	TEST_F(BasicComponentTest, ImageGrid9)
	{
		auto [img, imgPtr] = GetPtrs(CreateImageGrid9({ {170, 120}, Origin::LeftTop }, "BackImage9"));
		UIEx(m_panel).AttachComponent(move(img), { 400, 300 });
		EXPECT_TRUE(m_panel->BindTextureSourceInfo(m_sourceBinder.get(), nullptr));

		m_panel = WriteReadTest(m_panel, imgPtr);
		EXPECT_TRUE(m_panel->BindTextureSourceInfo(m_sourceBinder.get(), nullptr));

		CallMockRender(TestImageGrid9Render, 9);

		imgPtr->ChangeOrigin(Origin::Center);
		imgPtr->ChangeSize({ 180, 150 });

		CallMockRender(TestImageGrid9ChangeAreaRender, 9);
		
		SourceDivider srcDivider = GetSourceDivider(m_sourceBinder.get(), "BackImage9");
		EXPECT_TRUE((srcDivider.rect == Rectangle{ 10, 10, 64, 64 }));
		EXPECT_THAT(srcDivider.list, ElementsAre(30, 34, 36, 38));

		srcDivider.list.clear();
		srcDivider.list = { 20, 44, 26, 48 };
		EXPECT_TRUE(ModifyTextureSourceInfo(m_sourceBinder.get(), "BackImage9", srcDivider));

		m_panel = WriteReadTest(m_panel);
		EXPECT_TRUE(m_panel->BindTextureSourceInfo(m_sourceBinder.get(), nullptr));

		CallMockRender(TestImageGrid9ChangeAreaRender, 9);
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
		vector<wstring> bindKeys{ L"Hangle", L"English" };
		wstring text = L"<Hangle><Red>테스<br>트, 테스트2</Red>!@#$% </Hangle><English>Test. ^<Blue>&*</Blue>() End</English>";
		auto textArea = CreateTextArea({ {320, 120}, Origin::Center }, text, bindKeys);
		UIEx(m_panel).AttachComponent(move(textArea), { 400, 300 });
		m_panel = WriteReadTest(m_panel);
		EXPECT_TRUE(m_panel->BindTextureSourceInfo(m_sourceBinder.get(), m_renderer->GetTextureController()));

		CallMockRender(TestTextAreaRender);

		unique_ptr<UIComponent> clonePanel = m_panel->Clone();
		CallMockRender(clonePanel.get(), TestTextAreaRender);
		WriteReadTest(clonePanel);
	}
}

