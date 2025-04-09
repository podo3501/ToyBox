#include "pch.h"
#include "../ToyTestFixture.h"
#include "../Toy/UserInterface/TextureResourceBinder/TextureResourceBinder.h"
#include "../Toy/UserInterface/UIComponent/Components/TextureSwitcher.h"
#include "../Toy/UserInterface/UIComponent/Components/SampleComponent.h"
#include "../TestHelper.h"
#include "../Toy/Utility.h"

namespace UserInterfaceTest
{
	static void TestButton_PatchTexture1Render(size_t index, const RECT& dest, const RECT* source, TextureResourceBinder* rb)
	{
		vector<RECT> expectDest = { { 144, 104, 176, 136 } };
		TestCoordinates(index, dest, source, expectDest, GetSources(rb, "ExitButton1_Pressed"));
	}

	TEST_F(TextureSwitcherComponentTest, TextureSwitcher_PatchTexture1)
	{
		auto switcher = CreateTextureSwitcher({ {32, 32}, Origin::Center }, TextureSlice::One, GetStateKeyMap("ExitButton1"), BehaviorMode::Normal);
		UIEx(m_panel).AttachComponent(move(switcher), { 160, 120 });
		m_panel = WriteReadTest(m_panel);
		EXPECT_TRUE(m_panel->BindTextureSourceInfo(m_resBinder.get(), nullptr));

		MockMouseInput(144, 120, true);	//Pressed
		CallMockRender(TestButton_PatchTexture1Render, 1);

		FitToTextureSourceTest("ExitButton1_Pressed");
	}

	////////////////////////////////////////////////////////////////

	static void TestButton_PatchTexture3Render_H(size_t index, const RECT& dest, const RECT* source, TextureResourceBinder* rb)
	{
		vector<RECT> expectDest = { { 110, 96, 132, 144 }, { 132, 96, 188, 144 }, {188, 96, 210, 144} };
		TestCoordinates(index, dest, source, expectDest, GetSources(rb, "ScrollButton3_H_Hovered"));
	}

	static void TestButton_PatchTexture3ChangeAreaRender_H(size_t index, const RECT& dest, const RECT* source, TextureResourceBinder* rb)
	{
		vector<RECT> expectDest = { { 85, 96, 107, 144 }, { 107, 96, 213, 144 }, {213, 96, 235, 144} };
		TestCoordinates(index, dest, source, expectDest, GetSources(rb, "ScrollButton3_H_Normal"));
	}

	TEST_F(TextureSwitcherComponentTest, TextureSwitcher_PatchTexture3_H)
	{
		auto [switcher, switcherPtr] = GetPtrs(CreateTextureSwitcher({ {100, 48}, Origin::Center },
			TextureSlice::ThreeH, GetStateKeyMap("ScrollButton3_H"), BehaviorMode::Normal));
		UIEx(m_panel).AttachComponent(move(switcher), { 160, 120 });
		m_panel = WriteReadTest(m_panel, switcherPtr);
		EXPECT_TRUE(m_panel->BindTextureSourceInfo(m_resBinder.get(), nullptr));

		MockMouseInput(110, 96);	//Hover
		CallMockRender(TestButton_PatchTexture3Render_H, 3);

		switcherPtr->ChangeSize({ 150, 48 });
		MockMouseInput(0, 0);	//Normal
		CallMockRender(TestButton_PatchTexture3ChangeAreaRender_H, 3);

		CloneTest(m_panel.get(), TestButton_PatchTexture3ChangeAreaRender_H, 3);

		FitToTextureSourceTest("ScrollButton3_H_Normal");
	}

	////////////////////////////////////////////////////////////////

	static void TestButton_PatchTexture3Render_V(size_t index, const RECT& dest, const RECT* source, TextureResourceBinder* rb)
	{
		vector<RECT> expectDest = { { 76, 50, 124, 57 }, { 76, 57, 124, 143 }, {76, 143, 124, 150} };
		TestCoordinates(index, dest, source, expectDest, GetSources(rb, "ScrollButton3_V_Hovered"));
	}

	static void TestButton_PatchTexture3ChangeAreaRender_V(size_t index, const RECT& dest, const RECT* source, TextureResourceBinder* rb)
	{
		vector<RECT> expectDest = { { 76, 25, 124, 32 }, { 76, 32, 124, 168 }, {76, 168, 124, 175} };
		TestCoordinates(index, dest, source, expectDest, GetSources(rb, "ScrollButton3_V_Normal"));
	}

	TEST_F(TextureSwitcherComponentTest, TextureSwitcher_PatchTexture3_V)
	{
		auto [switcher, switcherPtr] = GetPtrs(CreateTextureSwitcher({ {48, 100}, Origin::Center },
			TextureSlice::ThreeV, GetStateKeyMap("ScrollButton3_V"), BehaviorMode::Normal));
		UIEx(m_panel).AttachComponent(move(switcher), { 100, 100 });
		m_panel = WriteReadTest(m_panel, switcherPtr);
		EXPECT_TRUE(m_panel->BindTextureSourceInfo(m_resBinder.get(), nullptr));

		MockMouseInput(77, 51);	//Hover
		CallMockRender(TestButton_PatchTexture3Render_V, 3);

		switcherPtr->ChangeSize({ 48, 150 });
		MockMouseInput(0, 0);	//Normal
		CallMockRender(TestButton_PatchTexture3ChangeAreaRender_V, 3);

		CloneTest(m_panel.get(), TestButton_PatchTexture3ChangeAreaRender_V, 3);

		FitToTextureSourceTest("ScrollButton3_V_Normal");
	}

	////////////////////////////////////////////////////////////////

	static void TestTextureSwitcher9(size_t index, const RECT& dest, const RECT* source, TextureResourceBinder* rb)
	{
		vector<RECT> expectDest = {
			{ 50, 50, 60, 60 }, { 60, 50, 140, 60 }, { 140, 50, 150, 60 },
			{ 50, 60, 60, 140 }, { 60, 60, 140, 140 }, { 140, 60, 150, 140 },
			{ 50, 140, 60, 150 }, { 60, 140, 140, 150 }, { 140, 140, 150, 150 } };
		TestCoordinates(index, dest, source, expectDest, GetSources(rb, "ListBackground9_Hovered"));
	}

	static void TestTextureSwitcher9_ChangeSize(size_t index, const RECT& dest, const RECT* source, TextureResourceBinder* rb)
	{
		vector<RECT> expectDest = {
			{ 25, 25, 35, 35 }, { 35, 25, 165, 35 }, { 165, 25, 175, 35 },
			{ 25, 35, 35, 165 }, { 35, 35, 165, 165 }, { 165, 35, 175, 165 },
			{ 25, 165, 35, 175 }, { 35, 165, 165, 175 }, { 165, 165, 175, 175 } };
		TestCoordinates(index, dest, source, expectDest, GetSources(rb, "ListBackground9_Normal"));
	}
		
	TEST_F(TextureSwitcherComponentTest, TextureSwitcher_PatchTexture9)
	{
		auto [switcher, switcherPtr] = GetPtrs(CreateTextureSwitcher({ {100, 100}, Origin::Center },
			TextureSlice::Nine, GetStateKeyMap("ListBackground9"), BehaviorMode::Normal));
		UIEx(m_panel).AttachComponent(move(switcher), { 100, 100 });
		EXPECT_TRUE(m_panel->BindTextureSourceInfo(m_resBinder.get(), nullptr)); //PatchTexture9일때에는 두번 하는 이유는 sourceInfo에서 크기를 알려줘야 한다.

		m_panel = WriteReadTest(m_panel, switcherPtr);
		EXPECT_TRUE(m_panel->BindTextureSourceInfo(m_resBinder.get(), nullptr));

		MockMouseInput(51, 51);	//Hover
		CallMockRender(TestTextureSwitcher9, 9);

		switcherPtr->ChangeSize({ 150, 150 });
		MockMouseInput(0, 0);	//Normal
		CallMockRender(TestTextureSwitcher9_ChangeSize, 9);

		CloneTest(m_panel.get(), TestTextureSwitcher9_ChangeSize, 9);

		FitToTextureSourceTest("ListBackground9_Normal");
	}
}