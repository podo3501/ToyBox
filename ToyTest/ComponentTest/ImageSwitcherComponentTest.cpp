#include "pch.h"
#include "../ToyTestFixture.h"
#include "../Toy/UserInterface/TextureResourceBinder/TextureResourceBinder.h"
#include "../Toy/UserInterface/UIComponent/Components/ImageSwitcher.h"
#include "../Toy/UserInterface/UIComponent/Components/SampleComponent.h"
#include "../TestHelper.h"
#include "../Toy/Utility.h"

namespace UserInterfaceTest
{
	static void TestButton_ImageGrid1Render(size_t index, const RECT& dest, const RECT* source, TextureResourceBinder* rb)
	{
		vector<RECT> expectDest = { { 144, 104, 176, 136 } };
		TestCoordinates(index, dest, source, expectDest, GetSources(rb, "ExitButton1_Pressed"));
	}

	TEST_F(ImageSwitcherComponentTest, ImageSwitcher_ImageGrid1)
	{
		auto switcher = CreateImageSwitcher({ {32, 32}, Origin::Center }, ImagePart::One, GetStateKeyMap("ExitButton1"), BehaviorMode::Normal);
		UIEx(m_panel).AttachComponent(move(switcher), { 160, 120 });
		m_panel = WriteReadTest(m_panel);
		EXPECT_TRUE(m_panel->BindTextureSourceInfo(m_resBinder.get(), nullptr));

		MockMouseInput(144, 120, true);	//Pressed
		CallMockRender(TestButton_ImageGrid1Render, 1);

		FitToTextureSourceTest("ExitButton1_Pressed");
	}

	////////////////////////////////////////////////////////////////

	static void TestButton_ImageGrid3Render_H(size_t index, const RECT& dest, const RECT* source, TextureResourceBinder* rb)
	{
		vector<RECT> expectDest = { { 110, 96, 132, 144 }, { 132, 96, 188, 144 }, {188, 96, 210, 144} };
		TestCoordinates(index, dest, source, expectDest, GetSources(rb, "ScrollButton3_H_Hovered"));
	}

	static void TestButton_ImageGrid3ChangeAreaRender_H(size_t index, const RECT& dest, const RECT* source, TextureResourceBinder* rb)
	{
		vector<RECT> expectDest = { { 85, 96, 107, 144 }, { 107, 96, 213, 144 }, {213, 96, 235, 144} };
		TestCoordinates(index, dest, source, expectDest, GetSources(rb, "ScrollButton3_H_Normal"));
	}

	TEST_F(ImageSwitcherComponentTest, ImageSwitcher_ImageGrid3_H)
	{
		auto [switcher, switcherPtr] = GetPtrs(CreateImageSwitcher({ {100, 48}, Origin::Center },
			ImagePart::ThreeH, GetStateKeyMap("ScrollButton3_H"), BehaviorMode::Normal));
		UIEx(m_panel).AttachComponent(move(switcher), { 160, 120 });
		m_panel = WriteReadTest(m_panel, switcherPtr);
		EXPECT_TRUE(m_panel->BindTextureSourceInfo(m_resBinder.get(), nullptr));

		MockMouseInput(110, 96);	//Hover
		CallMockRender(TestButton_ImageGrid3Render_H, 3);

		switcherPtr->ChangeSize({ 150, 48 });
		MockMouseInput(0, 0);	//Normal
		CallMockRender(TestButton_ImageGrid3ChangeAreaRender_H, 3);

		CloneTest(m_panel.get(), TestButton_ImageGrid3ChangeAreaRender_H, 3);

		FitToTextureSourceTest("ScrollButton3_H_Normal");
	}

	////////////////////////////////////////////////////////////////

	static void TestButton_ImageGrid3Render_V(size_t index, const RECT& dest, const RECT* source, TextureResourceBinder* rb)
	{
		vector<RECT> expectDest = { { 76, 50, 124, 57 }, { 76, 57, 124, 143 }, {76, 143, 124, 150} };
		TestCoordinates(index, dest, source, expectDest, GetSources(rb, "ScrollButton3_V_Hovered"));
	}

	static void TestButton_ImageGrid3ChangeAreaRender_V(size_t index, const RECT& dest, const RECT* source, TextureResourceBinder* rb)
	{
		vector<RECT> expectDest = { { 76, 25, 124, 32 }, { 76, 32, 124, 168 }, {76, 168, 124, 175} };
		TestCoordinates(index, dest, source, expectDest, GetSources(rb, "ScrollButton3_V_Normal"));
	}

	TEST_F(ImageSwitcherComponentTest, ImageSwitcher_ImageGrid3_V)
	{
		auto [switcher, switcherPtr] = GetPtrs(CreateImageSwitcher({ {48, 100}, Origin::Center },
			ImagePart::ThreeV, GetStateKeyMap("ScrollButton3_V"), BehaviorMode::Normal));
		UIEx(m_panel).AttachComponent(move(switcher), { 100, 100 });
		m_panel = WriteReadTest(m_panel, switcherPtr);
		EXPECT_TRUE(m_panel->BindTextureSourceInfo(m_resBinder.get(), nullptr));

		MockMouseInput(77, 51);	//Hover
		CallMockRender(TestButton_ImageGrid3Render_V, 3);

		switcherPtr->ChangeSize({ 48, 150 });
		MockMouseInput(0, 0);	//Normal
		CallMockRender(TestButton_ImageGrid3ChangeAreaRender_V, 3);

		CloneTest(m_panel.get(), TestButton_ImageGrid3ChangeAreaRender_V, 3);

		FitToTextureSourceTest("ScrollButton3_V_Normal");
	}
}