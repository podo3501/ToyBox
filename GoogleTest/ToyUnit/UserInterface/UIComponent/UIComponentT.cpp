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
}