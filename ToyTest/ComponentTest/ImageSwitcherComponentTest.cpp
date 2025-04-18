#include "pch.h"
#include "../ToyTestFixture.h"
#include "../Toy/UserInterface/TextureResourceBinder/TextureResourceBinder.h"
#include "../Toy/UserInterface/UIComponent/Components/TextureSwitcher.h"
#include "../Toy/UserInterface/UIComponent/Components/PatchTexture/PatchTexture.h"
#include "../Toy/UserInterface/UIComponent/Components/SampleComponent.h"
#include "../Toy/UserInterface/UIComponent/Components/PatchTexture/PatchTextureLite/PatchTextureLite.h"
#include "../TestHelper.h"
#include "../Toy/Utility.h"

namespace UserInterfaceTest
{
	static inline PatchTextureLite* GetPatchTextureLite(TextureSwitcher* switcherPtr) noexcept { return switcherPtr->GetPatchTextureLite(); }
	TEST_F(TextureSwitcherComponentTest, TextureSwitcher_PatchTextureLite1)
	{
		auto [switcher, switcherPtr] = GetPtrs(CreateTextureSwitcher({ {32, 32}, Origin::Center }, 
			TextureSlice::One, GetStateKeyMap("ExitButton1"), BehaviorMode::Normal));
		UIEx(m_panel).AttachComponent(move(switcher), { 160, 120 });
		m_panel = WriteReadTest(m_panel, switcherPtr);
		EXPECT_TRUE(m_panel->BindTextureSourceInfo(m_resBinder.get(), nullptr));
		EXPECT_EQ(GetPatchTextureLite(switcherPtr)->GetTextureSlice(), TextureSlice::One);

		MockMouseInput(144, 120, true);	//Pressed
		vector<RECT> exDest = { { 144, 104, 176, 136 } };
		TestMockRender(exDest, "ExitButton1_Pressed");
		EXPECT_EQ(*switcherPtr->GetState(), InteractState::Pressed);

		EXPECT_TRUE(switcherPtr->ChangeBindKey(m_resBinder.get(), "ListBackground1_Normal"));
		TestMockRender(exDest, "ListBackground1_Normal");
		
		FitToTextureSourceTest("ListBackground1_Normal");
	}

	////////////////////////////////////////////////////////////////

	TEST_F(TextureSwitcherComponentTest, TextureSwitcher_PatchTextureLite3_H)
	{
		auto [switcher, switcherPtr] = GetPtrs(CreateTextureSwitcher({ {100, 48}, Origin::Center },
			TextureSlice::ThreeH, GetStateKeyMap("ScrollButton3_H"), BehaviorMode::Normal));
		UIEx(m_panel).AttachComponent(move(switcher), { 160, 120 });
		m_panel = WriteReadTest(m_panel, switcherPtr);
		EXPECT_TRUE(m_panel->BindTextureSourceInfo(m_resBinder.get(), nullptr));
		EXPECT_EQ(GetPatchTextureLite(switcherPtr)->GetTextureSlice(), TextureSlice::ThreeH);

		MockMouseInput(110, 96);	//Hover
		vector<RECT> exDest = { { 110, 96, 132, 144 }, { 132, 96, 188, 144 }, {188, 96, 210, 144} };
		TestMockRender(exDest, "ScrollButton3_H_Hovered");

		switcherPtr->ChangeSize({ 150, 48 });
		MockMouseInput(0, 0);	//Normal
		vector<RECT> exChangeSize = { { 85, 96, 107, 144 }, { 107, 96, 213, 144 }, {213, 96, 235, 144} };
		TestMockRender(exChangeSize, "ScrollButton3_H_Normal");

		CloneTest(exChangeSize, "ScrollButton3_H_Normal");

		EXPECT_TRUE(switcherPtr->ChangeBindKey(m_resBinder.get(), "ScrollButton3_H_Hovered"));
		TestMockRender(exChangeSize, "ScrollButton3_H_Hovered");

		FitToTextureSourceTest("ScrollButton3_H_Hovered");
	}

	////////////////////////////////////////////////////////////////

	TEST_F(TextureSwitcherComponentTest, TextureSwitcher_PatchTextureLite3_V)
	{
		auto [switcher, switcherPtr] = GetPtrs(CreateTextureSwitcher({ {48, 100}, Origin::Center },
			TextureSlice::ThreeV, GetStateKeyMap("ScrollButton3_V"), BehaviorMode::Normal));
		UIEx(m_panel).AttachComponent(move(switcher), { 100, 100 });
		m_panel = WriteReadTest(m_panel, switcherPtr);
		EXPECT_TRUE(m_panel->BindTextureSourceInfo(m_resBinder.get(), nullptr));
		EXPECT_EQ(GetPatchTextureLite(switcherPtr)->GetTextureSlice(), TextureSlice::ThreeV);

		MockMouseInput(77, 51);	//Hover
		vector<RECT> exDest = { { 76, 50, 124, 57 }, { 76, 57, 124, 143 }, {76, 143, 124, 150} };
		TestMockRender(exDest, "ScrollButton3_V_Hovered");

		switcherPtr->ChangeSize({ 48, 150 });
		MockMouseInput(0, 0);	//Normal
		vector<RECT> exDestChange = { { 76, 25, 124, 32 }, { 76, 32, 124, 168 }, {76, 168, 124, 175} };
		TestMockRender(exDestChange, "ScrollButton3_V_Normal");

		CloneTest(exDestChange, "ScrollButton3_V_Normal");
		FitToTextureSourceTest("ScrollButton3_V_Normal");
	}

	////////////////////////////////////////////////////////////////
		
	TEST_F(TextureSwitcherComponentTest, TextureSwitcher_PatchTextureLite9)
	{
		auto [switcher, switcherPtr] = GetPtrs(CreateTextureSwitcher({ {100, 100}, Origin::Center },
			TextureSlice::Nine, GetStateKeyMap("ListBackground9"), BehaviorMode::Normal));
		UIEx(m_panel).AttachComponent(move(switcher), { 100, 100 });
		m_panel = WriteReadTest(m_panel, switcherPtr);
		EXPECT_TRUE(m_panel->BindTextureSourceInfo(m_resBinder.get(), nullptr));
		EXPECT_EQ(GetPatchTextureLite(switcherPtr)->GetTextureSlice(), TextureSlice::Nine);

		MockMouseInput(51, 51);	//Hover
		vector<RECT> exDest = {
			{ 50, 50, 60, 60 }, { 60, 50, 140, 60 }, { 140, 50, 150, 60 },
			{ 50, 60, 60, 140 }, { 60, 60, 140, 140 }, { 140, 60, 150, 140 },
			{ 50, 140, 60, 150 }, { 60, 140, 140, 150 }, { 140, 140, 150, 150 } };
		TestMockRender(exDest, "ListBackground9_Hovered");

		switcherPtr->ChangeSize({ 150, 150 });
		MockMouseInput(0, 0);	//Normal
		vector<RECT> exDestChange = {
			{ 25, 25, 35, 35 }, { 35, 25, 165, 35 }, { 165, 25, 175, 35 },
			{ 25, 35, 35, 165 }, { 35, 35, 165, 165 }, { 165, 35, 175, 165 },
			{ 25, 165, 35, 175 }, { 35, 165, 165, 175 }, { 165, 165, 175, 175 } };
		TestMockRender(exDestChange, "ListBackground9_Normal");

		CloneTest(exDestChange, "ListBackground9_Normal");
		FitToTextureSourceTest("ListBackground9_Normal");
	}
}