#include "pch.h"
#include "UIFixture.h"
#include "Helper.h"
#include "UserInterface/TextureResourceBinder/TextureResourceBinder.h"
#include "UserInterface/UIComponent/Components/TextureSwitcher.h"
#include "UserInterface/UIComponent/Components/PatchTexture/PatchTexture.h"
#include "UserInterface/UIComponent/Components/SampleComponent.h"
#include "UserInterface/UIComponent/Components/PatchTexture/PatchTextureLite/PatchTextureLite.h"
#include "UserInterface/UIComponent/Components/PatchTexture/PatchTextureLite/PatchTextureLite1.h"
#include "UserInterface/UIComponent/Components/PatchTexture/PatchTextureStd/PatchTextureStd1.h"
#include "Utils/CommonUtil.h"

namespace UserInterface
{
	static inline PatchTextureLite* GetPatchTextureLite(TextureSwitcher* switcherPtr) noexcept { return switcherPtr->GetPatchTextureLite(); }

	TEST_F(TextureSwitcherComponent, TextureSwitcher_PatchTextureLite1)
	{
		auto [switcher, switcherPtr] = GetPtrs(CreateComponent<TextureSwitcher>(TextureSlice::One,
			GetStateKeyMap("ExitButton1"), BehaviorMode::Normal));
		UIEx(m_main).AttachComponent(move(switcher), { 160, 120 });
		m_uiModule->BindTextureResources();
		tie(m_uiModule, m_main) = WriteReadTest(m_mockRenderer.get(), m_uiModule, switcherPtr);

		EXPECT_TRUE((switcherPtr->GetSize() == XMUINT2{ 32, 32 }));
		switcherPtr->ChangeOrigin(Origin::Center);
		switcherPtr->ChangeSize({ 50, 50 });

		EXPECT_EQ(GetPatchTextureLite(switcherPtr)->GetTextureSlice(), TextureSlice::One);
		MockMouseInput(144, 120, true);	//Pressed
		vector<RECT> exDest = { { 135, 95, 185, 145 } };
		TestMockRender(2, exDest, "ExitButton1_Pressed");
		EXPECT_EQ(*switcherPtr->GetState(), InteractState::Pressed);

		FitToTextureSourceTest("ExitButton1_Pressed");

		EXPECT_TRUE(switcherPtr->ChangeBindKey(GetResBinder(), "ListBackground1_Normal"));
		vector<RECT> exChangeKey = { { 144, 104, 176, 136 } };
		TestMockRender(2, exChangeKey, "ListBackground1_Normal");
	}

	////////////////////////////////////////////////////////////////

	TEST_F(TextureSwitcherComponent, TextureSwitcher_PatchTextureLite3_H)
	{
		auto [switcher, switcherPtr] = GetPtrs(CreateComponent<TextureSwitcher>(TextureSlice::ThreeH,
			GetStateKeyMap("ScrollButton3_H"), BehaviorMode::Normal));
		UIEx(m_main).AttachComponent(move(switcher), { 160, 120 });
		m_uiModule->BindTextureResources();
		tie(m_uiModule, m_main) = WriteReadTest(m_mockRenderer.get(), m_uiModule, switcherPtr);

		EXPECT_TRUE((switcherPtr->GetSize() == XMUINT2{ 48, 48 }));
		switcherPtr->ChangeOrigin(Origin::Center);
		switcherPtr->ChangeSize({ 100, 48 });

		EXPECT_EQ(GetPatchTextureLite(switcherPtr)->GetTextureSlice(), TextureSlice::ThreeH);
		MockMouseInput(110, 96);	//Hover
		vector<RECT> exDest = { { 110, 96, 132, 144 }, { 132, 96, 188, 144 }, { 188, 96, 210, 144 } };
		TestMockRender(2, exDest, "ScrollButton3_H_Hovered");

		switcherPtr->ChangeSize({ 150, 48 });
		MockMouseInput(0, 0);	//Normal
		vector<RECT> exChangeSize = { { 85, 96, 107, 144 }, { 107, 96, 213, 144 }, { 213, 96, 235, 144 } };
		TestMockRender(2, exChangeSize, "ScrollButton3_H_Normal");

		CloneTestForSwitcher(exChangeSize, "ScrollButton3_H_Normal");
		FitToTextureSourceTest("ScrollButton3_H_Normal");

		EXPECT_TRUE(switcherPtr->ChangeBindKey(GetResBinder(), "ScrollButton3_H_Hovered"));
		vector<RECT> exChangeKey = { { 136, 96, 158, 144 }, { 158, 96, 162, 144 }, { 162, 96, 184, 144 } };
		TestMockRender(2, exChangeKey, "ScrollButton3_H_Hovered");
	}

	////////////////////////////////////////////////////////////////

	TEST_F(TextureSwitcherComponent, TextureSwitcher_PatchTextureLite3_V)
	{
		auto [switcher, switcherPtr] = GetPtrs(CreateComponent<TextureSwitcher>(TextureSlice::ThreeV,
			GetStateKeyMap("ScrollButton3_V"), BehaviorMode::Normal));
		UIEx(m_main).AttachComponent(move(switcher), { 100, 100 });
		m_uiModule->BindTextureResources();
		tie(m_uiModule, m_main) = WriteReadTest(m_mockRenderer.get(), m_uiModule, switcherPtr);

		EXPECT_TRUE((switcherPtr->GetSize() == XMUINT2{ 16, 16 }));
		switcherPtr->ChangeOrigin(Origin::Center);
		switcherPtr->ChangeSize({ 48, 100 });

		EXPECT_EQ(GetPatchTextureLite(switcherPtr)->GetTextureSlice(), TextureSlice::ThreeV);
		MockMouseInput(77, 51);	//Hover
		vector<RECT> exDest = { { 76, 50, 124, 57 }, { 76, 57, 124, 143 }, {76, 143, 124, 150} };
		TestMockRender(2, exDest, "ScrollButton3_V_Hovered");

		switcherPtr->ChangeSize({ 48, 150 });
		MockMouseInput(0, 0);	//Normal
		vector<RECT> exDestChange = { { 76, 25, 124, 32 }, { 76, 32, 124, 168 }, {76, 168, 124, 175} };
		TestMockRender(2, exDestChange, "ScrollButton3_V_Normal");

		CloneTestForSwitcher(exDestChange, "ScrollButton3_V_Normal");
		FitToTextureSourceTest("ScrollButton3_V_Normal");

		EXPECT_TRUE(switcherPtr->ChangeBindKey(GetResBinder(), "ScrollButton3_V_Hovered"));
		vector<RECT> exChangeKey = { { 92, 92, 108, 99 }, { 92, 99, 108, 101 }, { 92, 101, 108, 108 } };
		TestMockRender(2, exChangeKey, "ScrollButton3_V_Hovered");
	}

	////////////////////////////////////////////////////////////////
		
	TEST_F(TextureSwitcherComponent, TextureSwitcher_PatchTextureLite9)
	{
		auto [switcher, switcherPtr] = GetPtrs(CreateComponent<TextureSwitcher>(TextureSlice::Nine,
			GetStateKeyMap("ListBackground9"), BehaviorMode::Normal));
		UIEx(m_main).AttachComponent(move(switcher), { 100, 100 });
		m_uiModule->BindTextureResources();
		tie(m_uiModule, m_main) = WriteReadTest(m_mockRenderer.get(), m_uiModule, switcherPtr);

		EXPECT_TRUE((switcherPtr->GetSize() == XMUINT2{ 32, 32 }));
		switcherPtr->ChangeOrigin(Origin::Center);
		switcherPtr->ChangeSize({ 100, 100 });

		EXPECT_EQ(GetPatchTextureLite(switcherPtr)->GetTextureSlice(), TextureSlice::Nine);
		MockMouseInput(51, 51);	//Hover
		vector<RECT> exDest = {
			{ 50, 50, 60, 60 }, { 60, 50, 140, 60 }, { 140, 50, 150, 60 },
			{ 50, 60, 60, 140 }, { 60, 60, 140, 140 }, { 140, 60, 150, 140 },
			{ 50, 140, 60, 150 }, { 60, 140, 140, 150 }, { 140, 140, 150, 150 } };
		TestMockRender(2, exDest, "ListBackground9_Hovered");

		switcherPtr->ChangeSize({ 150, 150 });
		MockMouseInput(0, 0);	//Normal
		vector<RECT> exDestChange = {
			{ 25, 25, 35, 35 }, { 35, 25, 165, 35 }, { 165, 25, 175, 35 },
			{ 25, 35, 35, 165 }, { 35, 35, 165, 165 }, { 165, 35, 175, 165 },
			{ 25, 165, 35, 175 }, { 35, 165, 165, 175 }, { 165, 165, 175, 175 } };
		TestMockRender(2, exDestChange, "ListBackground9_Normal");

		CloneTestForSwitcher(exDestChange, "ListBackground9_Normal");
		FitToTextureSourceTest("ListBackground9_Normal");

		EXPECT_TRUE(switcherPtr->ChangeBindKey(GetResBinder(), "OptionImage9"));
		vector<RECT> exChangeKey = {
			{ 23, 40, 43, 70 }, { 43, 40, 158, 70 }, { 158, 40, 178, 70 },
			{ 23, 70, 43, 142 }, { 43, 70, 158, 142 }, { 158, 70, 178, 142 },
			{ 23, 142, 43, 161 }, { 43, 142, 158, 161 }, { 158, 142, 178, 161 } };
		TestMockRender(3, exChangeKey, "OptionImage9");

		auto [tex, texPtr] = GetPtrs(CreateComponent<PatchTextureStd1>(UILayout{ {64, 64}, Origin::LeftTop }, "BackImage1"));
		EXPECT_EQ(switcherPtr->AttachComponentToCenter(move(tex), { 5, 5 }), nullptr);
		EXPECT_TRUE(UIEx(switcherPtr).FindComponent<decltype(texPtr)>(texPtr->GetName()));
	}
}