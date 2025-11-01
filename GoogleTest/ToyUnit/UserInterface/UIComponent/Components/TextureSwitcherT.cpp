#include "pch.h"
#include "TextureSwitcherT.h"
#include "UserInterface/UIComponent/Components/ComponentHelper.h"
#include "Toy/UserInterface/UIComponent/Components/PatchTexture/PatchTextureLite/PatchTextureLite.h"
#include "Shared/Utils/GeometryExt.h"

namespace UserInterfaceT::UIComponentT::ComponentT
{
	TEST_F(TextureSwitcherT, ChangeBindKey)
	{
		m_component->ChangeBindKey(GetResBinder(), "Button64_Normal");
		EXPECT_EQ(m_component->GetBindKey(), "Button64_Normal");
	}

	TEST_F(TextureSwitcherT, ChangeState)
	{
		m_component->ChangeState(InteractState::Hovered);
		EXPECT_EQ(m_component->GetState(), InteractState::Hovered);
	}

	TEST_F(TextureSwitcherT, ClearInteraction)
	{
		m_component->ChangeState(InteractState::Hovered);
		m_component->ClearInteraction();

		EXPECT_EQ(m_component->GetState(), InteractState::Normal);
	}

	TEST_F(TextureSwitcherT, Clone)
	{
		EXPECT_TRUE(TestClone(m_component));
	}

	TEST_F(TextureSwitcherT, FitToTextureSource)
	{
		auto preSize = m_component->GetSize();
		m_component->ChangeSize({ 10, 10 });

		m_component->FitToTextureSource();
		EXPECT_EQ(m_component->GetSize(), preSize);
	}

	TEST_F(TextureSwitcherT, GetSize)
	{
		EXPECT_TRUE((m_component->GetSize() == XMUINT2{ 30, 30 })); //초기셋팅은 Normal source 값을 가지고 와서 Size 크기로 셋팅한다.
	}

	TEST_F(TextureSwitcherT, OnHover)
	{
		m_component->OnHover();
		EXPECT_EQ(m_component->GetState(), InteractState::Hovered);
	}

	TEST_F(TextureSwitcherT, OnPress)
	{
		testing::MockFunction<void(InteractState)> OnPress;
		m_component->SetPressStateCB(OnPress.AsStdFunction());
		EXPECT_CALL(OnPress, Call(InteractState::Pressed)).Times(1);

		m_component->OnPress();

		EXPECT_EQ(m_component->GetState(), InteractState::Pressed);
	}

	TEST_F(TextureSwitcherT, ProcessRender)
	{
		RECT dest = RectangleToRect(m_component->GetArea());
		RECT source{ 0, 0, 30, 30 };

		MockTextureRender render;
		EXPECT_CALL(render, Render(0, dest, ::testing::Pointee(source))).Times(1);

		m_component->ProcessRender(&render);
	}

	TEST_F(TextureSwitcherT, WriteAndRead)
	{
		EXPECT_TRUE(TestWriteAndRead(m_component, GetTempDir() + L"TextureSwitcherT_WR.json", 
			GetResBinder(), GetTextureController()));
	}
}