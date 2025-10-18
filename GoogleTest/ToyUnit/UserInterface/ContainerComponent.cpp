#include "pch.h"
#include "Fixture/FixtureSuite.h"
#include "Helper.h"
#include "Mocks/MockInputManager.h"
#include "Shared/Utils/StlExt.h"
#include "Toy/UserInterface/UIComponent/Components/Container.h"
#include "Toy/UserInterface/UIComponent/Components/SampleComponent.h"

namespace UserInterface
{
	TEST_F(ContainerComponent, Container_PatchTextureStd1)
	{
		UILayout layout{ {32, 32}, Origin::Center };
		auto [container, containerPtr] = GetPtrs(CreateComponent<Container>(layout, GetComponentKeyMap(layout.GetSize(), "ExitButton1"), BehaviorMode::Normal));
		UIEx(m_main).AttachComponent(move(container), { 160, 120 });
		m_uiModule->BindTextureResources();
		tie(m_uiModule, m_main) = WriteReadTest(m_mockRenderer.get(), m_uiModule, containerPtr);

		m_mockInput->SetMouseState(144, 120, InputState::Pressed);
		TestMockRender(2, { { 144, 104, 176, 136 } }, "ExitButton1_Pressed");
	}

	////////////////////////////////////////////////////////////////

	TEST_F(ContainerComponent, Container_PatchTextureStd3_Horizontal)
	{
		UILayout layout{ {100, 48}, Origin::Center };
		auto [container, containerPtr] = GetPtrs(CreateComponent<Container>(layout,
			GetComponentKeyMap(DirectionType::Horizontal, layout.GetSize(), "ScrollButton3_H"), BehaviorMode::Normal));
		UIEx(m_main).AttachComponent(move(container), { 160, 120 });
		m_uiModule->BindTextureResources();
		tie(m_uiModule, m_main) = WriteReadTest(m_mockRenderer.get(), m_uiModule, containerPtr);

		m_mockInput->SetMouseState(110, 96); //Hover
		vector<RECT> exDest = { { 110, 96, 132, 144 }, { 132, 96, 188, 144 }, {188, 96, 210, 144} };
		TestMockRender(2, exDest, "ScrollButton3_H_Hovered");

		containerPtr->ChangeSize({ 150, 48 });
		m_mockInput->SetMouseState(0, 0); //Normal
		vector<RECT> exDestChange = { { 85, 96, 107, 144 }, { 107, 96, 213, 144 }, {213, 96, 235, 144} };
		TestMockRender(2, exDestChange, "ScrollButton3_H_Normal");

		CloneTest(exDestChange, "ScrollButton3_H_Normal");
	}

	////////////////////////////////////////////////////////////////

	TEST_F(ContainerComponent, Container_PatchTextureStd3_Vertical)
	{
		UILayout layout{ {48, 100}, Origin::Center };
		auto [container, containerPtr] = GetPtrs(CreateComponent<Container>(layout,
			GetComponentKeyMap(DirectionType::Vertical, layout.GetSize(), "ScrollButton3_V"), BehaviorMode::Normal));
		UIEx(m_main).AttachComponent(move(container), { 100, 100 });
		m_uiModule->BindTextureResources();
		tie(m_uiModule, m_main) = WriteReadTest(m_mockRenderer.get(), m_uiModule, containerPtr);

		m_mockInput->SetMouseState(77, 51); //Hover
		vector<RECT> exDest = { { 76, 50, 124, 57 }, { 76, 57, 124, 143 }, {76, 143, 124, 150} };
		TestMockRender(2, exDest, "ScrollButton3_V_Hovered");

		containerPtr->ChangeSize({ 48, 150 });
		m_mockInput->SetMouseState(0, 0); //Normal
		vector<RECT> exDestChange = { { 76, 25, 124, 32 }, { 76, 32, 124, 168 }, {76, 168, 124, 175} };
		TestMockRender(2, exDestChange, "ScrollButton3_V_Normal");

		CloneTest(exDestChange, "ScrollButton3_V_Normal");
	}
}