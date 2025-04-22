#include "pch.h"
#include "../ToyTestFixture.h"
#include "../IMockRenderer.h"
#include "../TestHelper.h"
#include "../Toy/UserInterface/UIComponent/Components/PatchTexture/PatchTextureStd/PatchTextureStd1.h"
#include "../Toy/UserInterface/UIComponent/Components/PatchTexture/PatchTextureStd/PatchTextureStd3.h"
#include "../Toy/UserInterface/UIComponent/Components/PatchTexture/PatchTextureStd/PatchTextureStd9.h"
#include "../Toy/UserInterface/UIComponent/Components/Container.h"
#include "../Toy/UserInterface/UIComponent/Components/TextArea.h"
#include "../Toy/UserInterface/UIComponent/Components/SampleComponent.h"
#include "../Toy/UserInterface/TextureResourceBinder/TextureResourceBinder.h"
#include "../Toy/Utility.h"

using testing::ElementsAre;

namespace UserInterfaceTest
{
	template<typename T>
	void FitToTextureSourceTest(UIComponent* panel,
		const string& componentName, const string& rectResourceName, TextureResourceBinder* resBinder)
	{
		T patchTex = UIEx(panel).FindComponent<T>(componentName);
		EXPECT_NE(patchTex, nullptr);

		EXPECT_TRUE(patchTex->FitToTextureSource());
		EXPECT_EQ(patchTex->GetSize(), GetSizeOfBindKey(resBinder, rectResourceName));
	}

	TEST_F(BasicComponentTest, Container_PatchTextureStd1)
	{
		UILayout layout{ {32, 32}, Origin::Center };
		auto container = CreateContainer(layout, GetComponentKeyMap(layout.GetSize(), "ExitButton1"), BehaviorMode::Normal);
		UIEx(m_panel).AttachComponent(move(container), { 160, 120 });
		EXPECT_TRUE(m_panel->BindTextureSourceInfo(m_resBinder.get(), nullptr));

		WriteReadTest(m_panel);
		EXPECT_TRUE(m_panel->BindTextureSourceInfo(m_resBinder.get(), nullptr));

		MockMouseInput(144, 120, true);	//Pressed
		
		TestMockRender(2, { { 144, 104, 176, 136 } }, "ExitButton1_Pressed");
	}

	////////////////////////////////////////////////////////////////

	TEST_F(BasicComponentTest, Container_PatchTextureStd3_Horizontal)
	{
		UILayout layout{ {100, 48}, Origin::Center };
		auto [container, containerBtr] = GetPtrs(CreateContainer(layout, 
			GetComponentKeyMap(DirectionType::Horizontal, layout.GetSize(), "ScrollButton3_H"), BehaviorMode::Normal));
		UIEx(m_panel).AttachComponent(move(container), { 160, 120 });
		EXPECT_TRUE(m_panel->BindTextureSourceInfo(m_resBinder.get(), nullptr));
		m_panel = WriteReadTest(m_panel, containerBtr);

		EXPECT_TRUE(m_panel->BindTextureSourceInfo(m_resBinder.get(), nullptr));
		MockMouseInput(110, 96);	//Hover
		vector<RECT> exDest = { { 110, 96, 132, 144 }, { 132, 96, 188, 144 }, {188, 96, 210, 144} };
		TestMockRender(2, exDest, "ScrollButton3_H_Hovered");

		containerBtr->ChangeSize({ 150, 48 });
		MockMouseInput(0, 0);	//Normal
		vector<RECT> exDestChange = { { 85, 96, 107, 144 }, { 107, 96, 213, 144 }, {213, 96, 235, 144} };
		TestMockRender(2, exDestChange, "ScrollButton3_H_Normal");

		CloneTest(exDestChange, "ScrollButton3_H_Normal");
	}

	////////////////////////////////////////////////////////////////

	TEST_F(BasicComponentTest, Container_PatchTextureStd3_Vertical)
	{
		UILayout layout{ {48, 100}, Origin::Center };
		auto [container, containerPtr] = GetPtrs(CreateContainer(layout, 
			GetComponentKeyMap(DirectionType::Vertical, layout.GetSize(), "ScrollButton3_V"), BehaviorMode::Normal));
		UIEx(m_panel).AttachComponent(move(container), { 100, 100 });
		EXPECT_TRUE(m_panel->BindTextureSourceInfo(m_resBinder.get(), nullptr));

		m_panel = WriteReadTest(m_panel, containerPtr);
		EXPECT_TRUE(m_panel->BindTextureSourceInfo(m_resBinder.get(), nullptr));

		MockMouseInput(77, 51);	//Hover
		vector<RECT> exDest = { { 76, 50, 124, 57 }, { 76, 57, 124, 143 }, {76, 143, 124, 150} };
		TestMockRender(2, exDest, "ScrollButton3_V_Hovered");

		containerPtr->ChangeSize({ 48, 150 });
		MockMouseInput(0, 0);	//Normal
		vector<RECT> exDestChange = { { 76, 25, 124, 32 }, { 76, 32, 124, 168 }, {76, 168, 124, 175} };
		TestMockRender(2, exDestChange, "ScrollButton3_V_Normal");

		CloneTest(exDestChange, "ScrollButton3_V_Normal");
	}

	////////////////////////////////////////////////////////

	TEST_F(BasicComponentTest, PatchTextureStd1)
	{
		auto [tex, img1Ptr] = GetPtrs(CreatePatchTextureStd1({ {64, 64}, Origin::Center }, "BackImage1"));
		UIEx(m_panel).AttachComponent(move(tex), { 400, 300 });
		m_panel = WriteReadTest(m_panel, img1Ptr);
		EXPECT_TRUE(m_panel->BindTextureSourceInfo(m_resBinder.get(), nullptr));
		EXPECT_EQ(*img1Ptr->GetTextureSlice(), TextureSlice::One);
		vector<RECT> exDest = { { 368, 268, 432, 332 } };
		TestMockRender(2, exDest, "BackImage1");

		img1Ptr->SetStateFlag(StateFlag::X_SizeLocked, true);
		img1Ptr->SetStateFlag(StateFlag::Y_SizeLocked, false);
		img1Ptr->ChangeSize({ 128, 64 });
		TestMockRender(2, exDest, "BackImage1");

		img1Ptr->SetStateFlag(StateFlag::X_SizeLocked, false);
		img1Ptr->SetStateFlag(StateFlag::Y_SizeLocked, true);
		img1Ptr->ChangeSize({ 64, 128 });
		TestMockRender(2, exDest, "BackImage1");

		img1Ptr->SetStateFlag(StateFlag::X_SizeLocked, true);
		img1Ptr->SetStateFlag(StateFlag::Y_SizeLocked, true);
		img1Ptr->ChangeSize({ 128, 128 });
		TestMockRender(2, exDest, "BackImage1");

		FitToTextureSourceTest<PatchTextureStd1*>(m_panel.get(), "PatchTextureStd1_0", "BackImage1", m_resBinder.get());

		EXPECT_TRUE(img1Ptr->ChangeBindKey(m_resBinder.get(), "ListBackground1_Normal"));
		vector<RECT> exChangeKey = { { 384, 284, 416, 316 } };
		TestMockRender(2, exChangeKey, "ListBackground1_Normal");
	}

	////////////////////////////////////////////////////////

	static bool ModifyTextureSourceInfo(TextureResourceBinder* rb, const string& key, const SourceDivider& srcDivider) noexcept
	{
		auto infoRef = rb->GetTextureSourceInfo(key);
		if (!infoRef) return false;

		TextureSourceInfo srcInfo{ infoRef->get() };
		srcInfo.sources = GetSources(srcInfo.texSlice, srcDivider);

		return rb->ModifyTextureSourceInfo(key, srcInfo);
	}

	TEST_F(BasicComponentTest, PatchTextureStd3_Horizontal)
	{
		auto [tex, texPtr] = GetPtrs(CreatePatchTextureStd3({ {100, 36}, Origin::LeftTop }, DirectionType::Horizontal, "ScrollButton3_H_Normal"));
		UIEx(m_panel).AttachComponent(move(tex), {400, 300});
		m_panel = WriteReadTest(m_panel, texPtr);
		EXPECT_TRUE(m_panel->BindTextureSourceInfo(m_resBinder.get(), nullptr));
		EXPECT_EQ(*texPtr->GetTextureSlice(), TextureSlice::ThreeH);

		vector<RECT> exDest = { { 400, 300, 422, 336 }, { 422, 300, 478, 336 }, { 478, 300, 500, 336 } };
		TestMockRender(2, exDest, "ScrollButton3_H_Normal");

		texPtr->ChangeOrigin(Origin::Center);
		texPtr->ChangeSize({ 120, 36 });

		vector<RECT> exDestChange = { { 340, 282, 362, 318 }, { 362, 282, 438, 318 }, { 438, 282, 460, 318 } };
		TestMockRender(2, exDestChange, "ScrollButton3_H_Normal");

		SourceDivider srcDivider = GetSourceDivider(m_resBinder.get(), "ScrollButton3_H_Normal");
		EXPECT_TRUE((srcDivider.rect == Rectangle{ 10, 82, 48, 48 }));
		EXPECT_THAT(srcDivider.list, ElementsAre(22, 26));

		srcDivider.list.clear();
		srcDivider.list = { 20, 28 };
		EXPECT_TRUE(ModifyTextureSourceInfo(m_resBinder.get(), "ScrollButton3_H_Normal", srcDivider));

		m_panel = WriteReadTest(m_panel, texPtr);
		EXPECT_TRUE(m_panel->BindTextureSourceInfo(m_resBinder.get(), nullptr));

		TestMockRender(2, exDestChange, "ScrollButton3_H_Normal");
		
		FitToTextureSourceTest<PatchTextureStd3*>(m_panel.get(), "PatchTextureStd3_0", "ScrollButton3_H_Normal", m_resBinder.get());

		EXPECT_TRUE(texPtr->ChangeBindKey(m_resBinder.get(), "ScrollButton3_H_Hovered"));
		vector<RECT> exChangeKey = { { 376, 276, 398, 324 }, { 398, 276, 402, 324 }, { 402, 276, 424, 324 } };
		TestMockRender(2, exChangeKey, "ScrollButton3_H_Hovered");
	}

	////////////////////////////////////////////////////////

	TEST_F(BasicComponentTest, PatchTextureStd3_Vertical)
	{
		auto [tex, img3Ptr] = GetPtrs(CreatePatchTextureStd3({ {36, 100}, Origin::LeftTop }, DirectionType::Vertical, "ScrollTrack3_V"));
		UIEx(m_panel).AttachComponent(move(tex), { 400, 300 });
		m_panel = WriteReadTest(m_panel, img3Ptr);
		EXPECT_TRUE(m_panel->BindTextureSourceInfo(m_resBinder.get(), nullptr));
		EXPECT_EQ(*img3Ptr->GetTextureSlice(), TextureSlice::ThreeV);

		vector<RECT> exDest = { { 400, 300, 436, 310 }, { 400, 310, 436, 390 }, { 400, 390, 436, 400 } };
		TestMockRender(2, exDest, "ScrollTrack3_V");

		img3Ptr->ChangeOrigin(Origin::Center);
		img3Ptr->ChangeSize({ 36, 120 });
		vector<RECT> exDestChange = { { 382, 240, 418, 250 }, { 382, 250, 418, 350 }, { 382, 350, 418, 360 } };
		TestMockRender(2, exDestChange, "ScrollTrack3_V");

		SourceDivider srcDivider = GetSourceDivider(m_resBinder.get(), "ScrollTrack3_V");
		EXPECT_TRUE((srcDivider.rect == Rectangle{ 114, 178, 16, 48 }));
		EXPECT_THAT(srcDivider.list, ElementsAre(10, 38));

		srcDivider.list.clear();
		srcDivider.list = { 14, 34 };
		EXPECT_TRUE(ModifyTextureSourceInfo(m_resBinder.get(), "ScrollTrack3_V", srcDivider));

		m_panel = WriteReadTest(m_panel, img3Ptr);
		EXPECT_TRUE(m_panel->BindTextureSourceInfo(m_resBinder.get(), nullptr));
		
		TestMockRender(2, exDestChange, "ScrollTrack3_V");

		FitToTextureSourceTest<PatchTextureStd3*>(m_panel.get(), "PatchTextureStd3_0", "ScrollTrack3_V", m_resBinder.get());

		EXPECT_TRUE(img3Ptr->ChangeBindKey(m_resBinder.get(), "ScrollButton3_V_Hovered"));
		vector<RECT> exChangeKey = { { 392, 292, 408, 299 }, { 392, 299, 408, 301 }, { 392, 301, 408, 308 } };
		TestMockRender(2, exChangeKey, "ScrollButton3_V_Hovered");
	}

	////////////////////////////////////////////////////////

	TEST_F(BasicComponentTest, PatchTextureStd9)
	{
		auto [tex, texPtr] = GetPtrs(CreatePatchTextureStd9({ {170, 120}, Origin::LeftTop }, "BackImage9"));
		UIEx(m_panel).AttachComponent(move(tex), { 400, 300 });
		m_panel = WriteReadTest(m_panel, texPtr);
		EXPECT_TRUE(m_panel->BindTextureSourceInfo(m_resBinder.get(), nullptr));
		EXPECT_EQ(*texPtr->GetTextureSlice(), TextureSlice::Nine);

		vector<RECT> exDest = {
			{ 400, 300, 430, 336 }, { 430, 300, 540, 336 }, {540, 300, 570, 336},
			{ 400, 336, 430, 394 }, { 430, 336, 540, 394 }, {540, 336, 570, 394},
			{ 400, 394, 430, 420 }, { 430, 394, 540, 420 }, {540, 394, 570, 420} };
		TestMockRender(2, exDest, "BackImage9");

		texPtr->ChangeOrigin(Origin::Center);
		texPtr->ChangeSize({ 180, 150 });

		vector<RECT> exDestChange = {
			{ 310, 225, 340, 261 }, { 340, 225, 460, 261 }, {460, 225, 490, 261},
			{ 310, 261, 340, 349 }, { 340, 261, 460, 349 }, {460, 261, 490, 349},
			{ 310, 349, 340, 375 }, { 340, 349, 460, 375 }, {460, 349, 490, 375} };
		TestMockRender(2, exDestChange, "BackImage9");
		
		SourceDivider srcDivider = GetSourceDivider(m_resBinder.get(), "BackImage9");
		EXPECT_TRUE((srcDivider.rect == Rectangle{ 10, 10, 64, 64 }));
		EXPECT_THAT(srcDivider.list, ElementsAre(30, 34, 36, 38));

		srcDivider.list.clear();
		srcDivider.list = { 20, 44, 30, 40 };
		EXPECT_TRUE(ModifyTextureSourceInfo(m_resBinder.get(), "BackImage9", srcDivider));

		m_panel = WriteReadTest(m_panel, texPtr);
		EXPECT_TRUE(m_panel->BindTextureSourceInfo(m_resBinder.get(), nullptr));

		TestMockRender(2, exDestChange, "BackImage9");

		FitToTextureSourceTest<PatchTextureStd9*>(m_panel.get(), "PatchTextureStd9_0", "BackImage9", m_resBinder.get());

		EXPECT_TRUE(texPtr->ChangeBindKey(m_resBinder.get(), "ListBackground9_Normal"));
		vector<RECT> exChangeKey = {
			{ 384, 284, 394, 294 }, { 394, 284, 406, 294 }, { 406, 284, 416, 294 },
			{ 384, 294, 394, 306 }, { 394, 294, 406, 306 }, { 406, 294, 416, 306 },
			{ 384, 306, 394, 316 }, { 394, 306, 406, 316 }, { 406, 306, 416, 316 } };
		TestMockRender(2, exChangeKey, "ListBackground9_Normal");
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
		EXPECT_TRUE(m_panel->BindTextureSourceInfo(m_resBinder.get(), m_renderer->GetTextureController()));

		CallMockRender(TestTextAreaRender);

		unique_ptr<UIComponent> clonePanel = m_panel->Clone();
		CallMockRender(clonePanel.get(), TestTextAreaRender);
		WriteReadTest(clonePanel);
	}
}

