#include "pch.h"
#include "../ToyTestFixture.h"
#include "../IMockRenderer.h"
#include "../TestHelper.h"
#include "UserInterface/UIComponent/Components/PatchTexture/PatchTextureStd/PatchTextureStd1.h"
#include "UserInterface/UIComponent/Components/PatchTexture/PatchTextureStd/PatchTextureStd3.h"
#include "UserInterface/UIComponent/Components/PatchTexture/PatchTextureStd/PatchTextureStd9.h"
#include "UserInterface/UIComponent/Components/Container.h"
#include "UserInterface/UIComponent/Components/TextArea.h"
#include "UserInterface/UIComponent/Components/SampleComponent.h"
#include "UserInterface/TextureResourceBinder/TextureResourceBinder.h"
#include "UserInterface/UIModule.h"
#include "Utils/CommonUtil.h"

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

	////////////////////////////////////////////////////////

	TEST_F(BasicComponentTest, PatchTextureStd1)
	{
		auto [tex, img1Ptr] = GetPtrs(CreateComponent<PatchTextureStd1>("BackImage1"));
		UIEx(m_main).AttachComponent(move(tex), { 400, 300 });
		m_uiModule->BindTextureResources();
		tie(m_uiModule, m_main) = WriteReadTest(m_renderer.get(), m_uiModule, img1Ptr);

		EXPECT_TRUE((img1Ptr->GetSize() == XMUINT2{ 64, 64 }));
		img1Ptr->ChangeOrigin(Origin::Center);
		
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

		FitToTextureSourceTest<PatchTextureStd1*>(m_main, "PatchTextureStd1", "BackImage1", GetResBinder());

		//EXPECT_TRUE(img1Ptr->ChangeBindKey(GetResBinder(), "ListBackground1_Normal"));
		//vector<RECT> exChangeKey = { { 384, 284, 416, 316 } };
		//TestMockRender(2, exChangeKey, "ListBackground1_Normal");
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
		auto [tex, texPtr] = GetPtrs(CreateComponent<PatchTextureStd3>(DirectionType::Horizontal, "ScrollButton3_H_Normal"));
		UIEx(m_main).AttachComponent(move(tex), { 400, 300 });
		m_uiModule->BindTextureResources();
		tie(m_uiModule, m_main) = WriteReadTest(m_renderer.get(), m_uiModule, texPtr);

		EXPECT_TRUE((texPtr->GetSize() == XMUINT2{ 48, 48 }));
		EXPECT_EQ(*texPtr->GetTextureSlice(), TextureSlice::ThreeH);
		vector<RECT> exDest = { { 400, 300, 422, 348 }, { 422, 300, 426, 348 }, { 426, 300, 448, 348 } };
		TestMockRender(2, exDest, "ScrollButton3_H_Normal");

		texPtr->ChangeOrigin(Origin::Center);
		texPtr->ChangeSize({ 120, 36 });

		vector<RECT> exDestChange = { { 340, 282, 362, 318 }, { 362, 282, 438, 318 }, { 438, 282, 460, 318 } };
		TestMockRender(2, exDestChange, "ScrollButton3_H_Normal");

		SourceDivider srcDivider = GetSourceDivider(GetResBinder(), "ScrollButton3_H_Normal");
		EXPECT_TRUE((srcDivider.rect == Rectangle{ 10, 82, 48, 48 }));
		EXPECT_THAT(srcDivider.list, ElementsAre(22, 26));

		srcDivider.list.clear();
		srcDivider.list = { 20, 28 };
		EXPECT_TRUE(ModifyTextureSourceInfo(GetResBinder(), "ScrollButton3_H_Normal", srcDivider));

		tie(m_uiModule, m_main) = WriteReadTest(m_renderer.get(), m_uiModule, texPtr);
		TestMockRender(2, exDestChange, "ScrollButton3_H_Normal");
		FitToTextureSourceTest<PatchTextureStd3*>(m_main, "PatchTextureStd3", "ScrollButton3_H_Normal", GetResBinder());

		EXPECT_TRUE(texPtr->ChangeBindKey(GetResBinder(), "ScrollButton3_H_Hovered"));
		vector<RECT> exChangeKey = { { 376, 276, 398, 324 }, { 398, 276, 402, 324 }, { 402, 276, 424, 324 } };
		TestMockRender(2, exChangeKey, "ScrollButton3_H_Hovered");
	}

	////////////////////////////////////////////////////////

	TEST_F(BasicComponentTest, PatchTextureStd3_Vertical)
	{
		auto [tex, img3Ptr] = GetPtrs(CreateComponent<PatchTextureStd3>(DirectionType::Vertical, "ScrollTrack3_V"));
		UIEx(m_main).AttachComponent(move(tex), { 400, 300 });
		m_uiModule->BindTextureResources();
		tie(m_uiModule, m_main) = WriteReadTest(m_renderer.get(), m_uiModule, img3Ptr);

		EXPECT_TRUE((img3Ptr->GetSize() == XMUINT2{ 16, 48 }));
		EXPECT_EQ(*img3Ptr->GetTextureSlice(), TextureSlice::ThreeV);
		vector<RECT> exDest = { { 400, 300, 416, 310 }, { 400, 310, 416, 338 }, { 400, 338, 416, 348 } };
		TestMockRender(2, exDest, "ScrollTrack3_V");

		img3Ptr->ChangeOrigin(Origin::Center);
		img3Ptr->ChangeSize({ 36, 120 });
		vector<RECT> exDestChange = { { 382, 240, 418, 250 }, { 382, 250, 418, 350 }, { 382, 350, 418, 360 } };
		TestMockRender(2, exDestChange, "ScrollTrack3_V");

		SourceDivider srcDivider = GetSourceDivider(GetResBinder(), "ScrollTrack3_V");
		EXPECT_TRUE((srcDivider.rect == Rectangle{ 114, 178, 16, 48 }));
		EXPECT_THAT(srcDivider.list, ElementsAre(10, 38));

		srcDivider.list.clear();
		srcDivider.list = { 14, 34 };
		EXPECT_TRUE(ModifyTextureSourceInfo(GetResBinder(), "ScrollTrack3_V", srcDivider));

		tie(m_uiModule, m_main) = WriteReadTest(m_renderer.get(), m_uiModule, img3Ptr);
		TestMockRender(2, exDestChange, "ScrollTrack3_V");
		FitToTextureSourceTest<PatchTextureStd3*>(m_main, "PatchTextureStd3", "ScrollTrack3_V", GetResBinder());

		EXPECT_TRUE(img3Ptr->ChangeBindKey(GetResBinder(), "ScrollButton3_V_Hovered"));
		vector<RECT> exChangeKey = { { 392, 292, 408, 299 }, { 392, 299, 408, 301 }, { 392, 301, 408, 308 } };
		TestMockRender(2, exChangeKey, "ScrollButton3_V_Hovered");
	}

	////////////////////////////////////////////////////////

	TEST_F(BasicComponentTest, PatchTextureStd9)
	{
		auto [tex, texPtr] = GetPtrs(CreateComponent<PatchTextureStd9>("BackImage9"));
		UIEx(m_main).AttachComponent(move(tex), { 400, 300 });
		m_uiModule->BindTextureResources();
		tie(m_uiModule, m_main) = WriteReadTest(m_renderer.get(), m_uiModule, texPtr);

		EXPECT_TRUE((texPtr->GetSize() == XMUINT2{ 64, 64 }));
		EXPECT_EQ(*texPtr->GetTextureSlice(), TextureSlice::Nine);
		vector<RECT> exDest = {
			{ 400, 300, 430, 336 }, { 430, 300, 434, 336 }, { 434, 300, 464, 336 },
			{ 400, 336, 430, 338 }, { 430, 336, 434, 338 }, { 434, 336, 464, 338 },
			{ 400, 338, 430, 364 }, { 430, 338, 434, 364 }, { 434, 338, 464, 364 } };
		TestMockRender(2, exDest, "BackImage9");

		texPtr->ChangeOrigin(Origin::Center);
		texPtr->ChangeSize({ 180, 150 });

		vector<RECT> exDestChange = {
			{ 310, 225, 340, 261 }, { 340, 225, 460, 261 }, { 460, 225, 490, 261 },
			{ 310, 261, 340, 349 }, { 340, 261, 460, 349 }, { 460, 261, 490, 349 },
			{ 310, 349, 340, 375 }, { 340, 349, 460, 375 }, { 460, 349, 490, 375 } };
		TestMockRender(2, exDestChange, "BackImage9");
		
		SourceDivider srcDivider = GetSourceDivider(GetResBinder(), "BackImage9");
		EXPECT_TRUE((srcDivider.rect == Rectangle{ 10, 10, 64, 64 }));
		EXPECT_THAT(srcDivider.list, ElementsAre(30, 34, 36, 38));

		srcDivider.list.clear();
		srcDivider.list = { 20, 44, 30, 40 };
		EXPECT_TRUE(ModifyTextureSourceInfo(GetResBinder(), "BackImage9", srcDivider));

		tie(m_uiModule, m_main) = WriteReadTest(m_renderer.get(), m_uiModule, texPtr);
		TestMockRender(2, exDestChange, "BackImage9");
		FitToTextureSourceTest<PatchTextureStd9*>(m_main, "PatchTextureStd9", "BackImage9", GetResBinder());

		EXPECT_TRUE(texPtr->ChangeBindKey(GetResBinder(), "ListBackground9_Normal"));
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
		auto [textArea, textAreaPtr] = GetPtrs(CreateComponent<TextArea>(text, bindKeys));
		UIEx(m_main).AttachComponent(move(textArea), { 400, 300 });
		m_uiModule->BindTextureResources();
		tie(m_uiModule, m_main) = WriteReadTest(m_renderer.get(), m_uiModule, textAreaPtr);

		EXPECT_TRUE((textAreaPtr->GetSize() == XMUINT2{}));
		textAreaPtr->ChangeSize(320, 120);
		textAreaPtr->ChangeOrigin(Origin::Center);
		CallMockRender(TestTextAreaRender);

		unique_ptr<UIComponent> clonePanel = m_main->Clone();
		CallMockRender(clonePanel.get(), TestTextAreaRender);
		tie(m_uiModule, m_main) = WriteReadTest(m_renderer.get(), m_uiModule, textAreaPtr);
	}
}

