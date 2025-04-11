#include "pch.h"
#include "../ToyTestFixture.h"
#include "../IMockRenderer.h"
#include "../TestHelper.h"
#include "../Toy/UserInterface/UIComponent/Components/PatchTexture/PatchTexture1.h"
#include "../Toy/UserInterface/UIComponent/Components/PatchTexture/PatchTexture3.h"
#include "../Toy/UserInterface/UIComponent/Components/PatchTexture/PatchTexture9.h"
#include "../Toy/UserInterface/UIComponent/Components/Container.h"
#include "../Toy/UserInterface/UIComponent/Components/TextArea.h"
#include "../Toy/UserInterface/UIComponent/Components/SampleComponent.h"
#include "../Toy/UserInterface/TextureResourceBinder/TextureResourceBinder.h"
#include "../Toy/Utility.h"

using testing::ElementsAre;

namespace UserInterfaceTest
{
	static inline XMUINT2 GetSizeFromRectangles(const vector<Rectangle>& rectangles) noexcept
	{
		return GetSizeFromRectangle(CombineRectangles(rectangles));
	}

	template<typename T>
	void FitToTextureSourceTest(UIComponent* panel,
		const string& componentName, const string& rectResourceName, TextureResourceBinder* resBinder)
	{
		T patchTex = UIEx(panel).FindComponent<T>(componentName);
		EXPECT_NE(patchTex, nullptr);

		EXPECT_TRUE(patchTex->FitToTextureSource());

		auto rectRef = GetRectangles(resBinder, rectResourceName);
		EXPECT_NE(rectRef, nullopt);

		EXPECT_EQ(patchTex->GetSize(), GetSizeFromRectangles(rectRef->get()));
	}

	TEST_F(BasicComponentTest, Container_PatchTexture1)
	{
		UILayout layout{ {32, 32}, Origin::Center };
		auto container = CreateContainer(layout, GetComponentKeyMap(layout.GetSize(), "ExitButton1"), BehaviorMode::Normal);
		UIEx(m_panel).AttachComponent(move(container), { 160, 120 });
		WriteReadTest(m_panel);
		EXPECT_TRUE(m_panel->BindTextureSourceInfo(m_resBinder.get(), nullptr));

		MockMouseInput(144, 120, true);	//Pressed
		
		TestMockRender({ { 144, 104, 176, 136 } }, "ExitButton1_Pressed");
	}

	////////////////////////////////////////////////////////////////

	TEST_F(BasicComponentTest, Container_PatchTexture3_Horizontal)
	{
		UILayout layout{ {100, 48}, Origin::Center };
		auto [container, containerBtr] = GetPtrs(CreateContainer(layout, 
			GetComponentKeyMap(DirectionType::Horizontal, layout.GetSize(), "ScrollButton3_H"), BehaviorMode::Normal));
		UIEx(m_panel).AttachComponent(move(container), { 160, 120 });
		m_panel = WriteReadTest(m_panel, containerBtr);
		EXPECT_TRUE(m_panel->BindTextureSourceInfo(m_resBinder.get(), nullptr));

		MockMouseInput(110, 96);	//Hover
		vector<RECT> exDest = { { 110, 96, 132, 144 }, { 132, 96, 188, 144 }, {188, 96, 210, 144} };
		TestMockRender(exDest, "ScrollButton3_H_Hovered");

		containerBtr->ChangeSize({ 150, 48 });
		MockMouseInput(0, 0);	//Normal
		vector<RECT> exDestChange = { { 85, 96, 107, 144 }, { 107, 96, 213, 144 }, {213, 96, 235, 144} };
		TestMockRender(exDestChange, "ScrollButton3_H_Normal");

		CloneTest(exDestChange, "ScrollButton3_H_Normal");
	}

	////////////////////////////////////////////////////////////////

	TEST_F(BasicComponentTest, Container_PatchTexture3_Vertical)
	{
		UILayout layout{ {48, 100}, Origin::Center };
		auto [container, containerPtr] = GetPtrs(CreateContainer(layout, 
			GetComponentKeyMap(DirectionType::Vertical, layout.GetSize(), "ScrollButton3_V"), BehaviorMode::Normal));
		UIEx(m_panel).AttachComponent(move(container), { 100, 100 });
		m_panel = WriteReadTest(m_panel, containerPtr);
		EXPECT_TRUE(m_panel->BindTextureSourceInfo(m_resBinder.get(), nullptr));

		MockMouseInput(77, 51);	//Hover
		vector<RECT> exDest = { { 76, 50, 124, 57 }, { 76, 57, 124, 143 }, {76, 143, 124, 150} };
		TestMockRender(exDest, "ScrollButton3_V_Hovered");

		containerPtr->ChangeSize({ 48, 150 });
		MockMouseInput(0, 0);	//Normal
		vector<RECT> exDestChange = { { 76, 25, 124, 32 }, { 76, 32, 124, 168 }, {76, 168, 124, 175} };
		TestMockRender(exDestChange, "ScrollButton3_V_Normal");

		CloneTest(exDestChange, "ScrollButton3_V_Normal");
	}

	////////////////////////////////////////////////////////

	TEST_F(BasicComponentTest, PatchTexture1)
	{
		auto [tex, img1Ptr] = GetPtrs(CreatePatchTexture1({ {64, 64}, Origin::Center }, "BackImage1"));
		UIEx(m_panel).AttachComponent(move(tex), { 400, 300 });
		m_panel = WriteReadTest(m_panel, img1Ptr);
		EXPECT_TRUE(m_panel->BindTextureSourceInfo(m_resBinder.get(), nullptr));
		vector<RECT> exDest = { { 368, 268, 432, 332 } };
		TestMockRender(exDest, "BackImage1");

		img1Ptr->SetStateFlag(StateFlag::X_SizeLocked, true);
		img1Ptr->SetStateFlag(StateFlag::Y_SizeLocked, false);
		img1Ptr->ChangeSize({ 128, 64 });
		TestMockRender(exDest, "BackImage1");

		img1Ptr->SetStateFlag(StateFlag::X_SizeLocked, false);
		img1Ptr->SetStateFlag(StateFlag::Y_SizeLocked, true);
		img1Ptr->ChangeSize({ 64, 128 });
		TestMockRender(exDest, "BackImage1");

		img1Ptr->SetStateFlag(StateFlag::X_SizeLocked, true);
		img1Ptr->SetStateFlag(StateFlag::Y_SizeLocked, true);
		img1Ptr->ChangeSize({ 128, 128 });
		TestMockRender(exDest, "BackImage1");

		FitToTextureSourceTest<PatchTexture1*>(m_panel.get(), "PatchTexture1_0", "BackImage1", m_resBinder.get());
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

	TEST_F(BasicComponentTest, PatchTexture3_Horizontal)
	{
		auto [tex, texPtr] = GetPtrs(CreatePatchTexture3({ {100, 36}, Origin::LeftTop }, DirectionType::Horizontal, "ScrollButton3_H_Normal"));
		UIEx(m_panel).AttachComponent(move(tex), {400, 300});
		m_panel = WriteReadTest(m_panel, texPtr);
		EXPECT_TRUE(m_panel->BindTextureSourceInfo(m_resBinder.get(), nullptr));

		vector<RECT> exDest = { { 400, 300, 422, 336 }, { 422, 300, 478, 336 }, {478, 300, 500, 336} };
		TestMockRender(exDest, "ScrollButton3_H_Normal");

		texPtr->ChangeOrigin(Origin::Center);
		texPtr->ChangeSize({ 120, 36 });

		vector<RECT> exDestChange = { { 340, 282, 362, 318 }, { 362, 282, 438, 318 }, {438, 282, 460, 318} };
		TestMockRender(exDestChange, "ScrollButton3_H_Normal");

		SourceDivider srcDivider = GetSourceDivider(m_resBinder.get(), "ScrollButton3_H_Normal");
		EXPECT_TRUE((srcDivider.rect == Rectangle{ 10, 82, 48, 48 }));
		EXPECT_THAT(srcDivider.list, ElementsAre(22, 26));

		srcDivider.list.clear();
		srcDivider.list = { 20, 28 };
		EXPECT_TRUE(ModifyTextureSourceInfo(m_resBinder.get(), "ScrollButton3_H_Normal", srcDivider));

		m_panel = WriteReadTest(m_panel);
		EXPECT_TRUE(m_panel->BindTextureSourceInfo(m_resBinder.get(), nullptr));

		TestMockRender(exDestChange, "ScrollButton3_H_Normal");
		
		FitToTextureSourceTest<PatchTexture3*>(m_panel.get(), "PatchTexture3_0", "ScrollButton3_H_Normal", m_resBinder.get());
	}

	////////////////////////////////////////////////////////

	TEST_F(BasicComponentTest, PatchTexture3_Vertical)
	{
		auto [tex, img3Ptr] = GetPtrs(CreatePatchTexture3({ {36, 100}, Origin::LeftTop }, DirectionType::Vertical, "ScrollTrack3_V"));
		UIEx(m_panel).AttachComponent(move(tex), { 400, 300 });
		m_panel = WriteReadTest(m_panel, img3Ptr);
		EXPECT_TRUE(m_panel->BindTextureSourceInfo(m_resBinder.get(), nullptr));

		vector<RECT> exDest = { { 400, 300, 436, 310 }, { 400, 310, 436, 390 }, { 400, 390, 436, 400 } };
		TestMockRender(exDest, "ScrollTrack3_V");

		img3Ptr->ChangeOrigin(Origin::Center);
		img3Ptr->ChangeSize({ 36, 120 });
		vector<RECT> exDestChange = { { 382, 240, 418, 250 }, { 382, 250, 418, 350 }, { 382, 350, 418, 360 } };
		TestMockRender(exDestChange, "ScrollTrack3_V");

		SourceDivider srcDivider = GetSourceDivider(m_resBinder.get(), "ScrollTrack3_V");
		EXPECT_TRUE((srcDivider.rect == Rectangle{ 114, 178, 16, 48 }));
		EXPECT_THAT(srcDivider.list, ElementsAre(10, 38));

		srcDivider.list.clear();
		srcDivider.list = { 14, 34 };
		EXPECT_TRUE(ModifyTextureSourceInfo(m_resBinder.get(), "ScrollTrack3_V", srcDivider));

		m_panel = WriteReadTest(m_panel);
		EXPECT_TRUE(m_panel->BindTextureSourceInfo(m_resBinder.get(), nullptr));
		
		TestMockRender(exDestChange, "ScrollTrack3_V");

		FitToTextureSourceTest<PatchTexture3*>(m_panel.get(), "PatchTexture3_0", "ScrollTrack3_V", m_resBinder.get());
	}

	////////////////////////////////////////////////////////

	TEST_F(BasicComponentTest, PatchTexture9)
	{
		auto [tex, texPtr] = GetPtrs(CreatePatchTexture9({ {170, 120}, Origin::LeftTop }, "BackImage9"));
		UIEx(m_panel).AttachComponent(move(tex), { 400, 300 });
		EXPECT_TRUE(m_panel->BindTextureSourceInfo(m_resBinder.get(), nullptr));

		m_panel = WriteReadTest(m_panel, texPtr);
		EXPECT_TRUE(m_panel->BindTextureSourceInfo(m_resBinder.get(), nullptr));

		vector<RECT> exDest = {
			{ 400, 300, 430, 336 }, { 430, 300, 540, 336 }, {540, 300, 570, 336},
			{ 400, 336, 430, 394 }, { 430, 336, 540, 394 }, {540, 336, 570, 394},
			{ 400, 394, 430, 420 }, { 430, 394, 540, 420 }, {540, 394, 570, 420} };
		TestMockRender(exDest, "BackImage9");

		texPtr->ChangeOrigin(Origin::Center);
		texPtr->ChangeSize({ 180, 150 });

		vector<RECT> exDestChange = {
			{ 310, 225, 340, 261 }, { 340, 225, 460, 261 }, {460, 225, 490, 261},
			{ 310, 261, 340, 349 }, { 340, 261, 460, 349 }, {460, 261, 490, 349},
			{ 310, 349, 340, 375 }, { 340, 349, 460, 375 }, {460, 349, 490, 375} };
		TestMockRender(exDestChange, "BackImage9");
		
		SourceDivider srcDivider = GetSourceDivider(m_resBinder.get(), "BackImage9");
		EXPECT_TRUE((srcDivider.rect == Rectangle{ 10, 10, 64, 64 }));
		EXPECT_THAT(srcDivider.list, ElementsAre(30, 34, 36, 38));

		srcDivider.list.clear();
		srcDivider.list = { 20, 44, 30, 40 };
		EXPECT_TRUE(ModifyTextureSourceInfo(m_resBinder.get(), "BackImage9", srcDivider));

		m_panel = WriteReadTest(m_panel);
		EXPECT_TRUE(m_panel->BindTextureSourceInfo(m_resBinder.get(), nullptr));

		TestMockRender(exDestChange, "BackImage9");

		FitToTextureSourceTest<PatchTexture9*>(m_panel.get(), "PatchTexture9_0", "BackImage9", m_resBinder.get());
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

