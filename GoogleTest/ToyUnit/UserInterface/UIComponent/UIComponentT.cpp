#include "pch.h"
#include "UIComponentT.h"

namespace UserInterfaceT::UIComponentT
{
	TEST_F(UIComponentT, ChangeOrigin)
	{
		VerifyTransformChange(
			[this] { m_parent->ChangeOrigin(Origin::Center); },
			ChangeExpect::ParentLeftTopChanged
		);
	}

	TEST_F(UIComponentT, ChangeRelativePosition)
	{
		VerifyTransformChange(
			[this] { m_parent->ChangeRelativePosition({ 100, 100 }); },
			ChangeExpect::ParentLeftTopChanged |
			ChangeExpect::ParentRelativeChanged
		);
	}

	TEST_F(UIComponentT, ChangeSize_Center)
	{
		m_parent->ChangeOrigin(Origin::Center);

		VerifyTransformChange(
			[this] { m_parent->ChangeSize({ 200, 200 }); },
			ChangeExpect::ParentLeftTopChanged |
			ChangeExpect::ParentSizeChanged |
			ChangeExpect::ChildRelativeChanged
		);
	}

	TEST_F(UIComponentT, ChangeSize_LeftTop) //LeftTop 일때 변환
	{
		VerifyTransformChange(
			[this] { m_parent->ChangeSize({ 200, 200 }); },
			ChangeExpect::ParentSizeChanged |
			ChangeExpect::ChildRelativeChanged
		);
	}

	TEST_F(UIComponentT, ChangeSize_LockPosOnResize) //StateFlag::LockPosOnResize 일때 변환
	{
		m_parent->SetStateFlag(StateFlag::LockPosOnResize, true);

		VerifyTransformChange(
			[this] { m_parent->ChangeSize({ 200, 200 }); },
			ChangeExpect::ParentSizeChanged
		);
	}

	TEST_F(UIComponentT, StateFlag_Attach)
	{
		m_main->SetStateFlag(StateFlag::Attach, false);	//attach 불가
		auto [owner, _] = CreateMockComponent<MockComponent>();
		unique_ptr<UIComponent> result = UIEx(m_main).AttachComponent(move(owner));

		EXPECT_NE(result, nullptr); //attach가 안되면 nullptr 값이 아니다.
	}

	TEST_F(UIComponentT, StateFlag_Detach)
	{
		auto [owner, component] = CreateMockComponent<MockComponent>();
		UIEx(m_main).AttachComponent(move(owner));
		m_main->SetStateFlag(StateFlag::Detach, false);
		auto [detach, detachPtr] = UIEx(component).DetachComponent();

		EXPECT_EQ(detach, nullptr); //detach가 안되면 nullptr이 반환된다.
		EXPECT_EQ(detachPtr, nullptr);
	}

	TEST_F(UIComponentT, StateFlag_SizeLocked)
	{
		m_main->ChangeSize(10, 10);
		m_main->SetStateFlag(StateFlag::X_SizeLocked, true);
		m_main->SetStateFlag(StateFlag::Y_SizeLocked, true);
		m_main->ChangeSize(20, 20);

		EXPECT_EQ(m_main->GetSize(), XMUINT2(10, 10));
	}
}