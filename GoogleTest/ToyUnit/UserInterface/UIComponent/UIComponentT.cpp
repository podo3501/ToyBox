#include "pch.h"
#include "UIComponentT.h"

namespace UserInterface
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

	TEST_F(UIComponentT, ChangeSize_LeftTop) //LeftTop �϶� ��ȯ
	{
		VerifyTransformChange(
			[this] { m_parent->ChangeSize({ 200, 200 }); },
			ChangeExpect::ParentSizeChanged |
			ChangeExpect::ChildRelativeChanged
		);
	}

	TEST_F(UIComponentT, ChangeSize_LockPosOnResize) //StateFlag::LockPosOnResize �϶� ��ȯ
	{
		m_parent->SetStateFlag(StateFlag::LockPosOnResize, true);

		VerifyTransformChange(
			[this] { m_parent->ChangeSize({ 200, 200 }); },
			ChangeExpect::ParentSizeChanged
		);
	}

	TEST_F(UIComponentT, FindComponent)
	{
		UIEx(m_parent).RenameRegion("Region1");
		UIEx(m_parent).Rename("Parent"); 
		UIEx(m_child).Rename("Child");

		//False�� ���� ã�� Component�� �ٸ� Region�̱� ������ ã�� �� ����.
		EXPECT_FALSE(UIEx(m_main).FindComponent("Parent"));
		EXPECT_FALSE(UIEx(m_main).FindComponent("Child"));

		EXPECT_FALSE(UIEx(m_parent).FindComponent("Main"));
		EXPECT_TRUE(UIEx(m_parent).FindComponent("Child"));

		EXPECT_FALSE(UIEx(m_child).FindComponent("Main"));
		EXPECT_TRUE(UIEx(m_child).FindComponent("Parent"));
	}

	TEST_F(UIComponentT, FindRegionComponent)
	{
		UIEx(m_parent).RenameRegion("Region1");
		UIEx(m_child).RenameRegion("Region2");

		//Region�� �ִ� Component�� ��� �����θ� ã�� ���δ� ã�� �ʴ´�.
		EXPECT_EQ(UIEx(m_main).FindRegionComponent("Region1"), m_parent);
		EXPECT_FALSE(UIEx(m_main).FindRegionComponent("Region2"));

		EXPECT_EQ(UIEx(m_parent).FindRegionComponent("Region2"), m_child);
		EXPECT_FALSE(UIEx(m_child).FindRegionComponent("Region1"));
	}

	TEST_F(UIComponentT, FindRenderComponents)
	{
		vector<UIComponent*> compAtParent = UIEx(m_main).FindRenderComponents(m_parent->GetLeftTop());
		EXPECT_EQ(compAtParent[0], m_main.get());
		EXPECT_EQ(compAtParent[1], m_parent);

		vector<UIComponent*> compAtChild = UIEx(m_main).FindRenderComponents(m_child->GetLeftTop());
		EXPECT_EQ(compAtChild[2], m_child);
	}

	TEST_F(UIComponentT, GetChildrenBoundsSize)
	{
		XMUINT2 preSize = UIEx(m_parent).GetChildrenBoundsSize();
		auto [detached, parent] = UIEx(m_parent).DetachComponent(); //detached�� m_parent, parent�� m_main
		detached->UpdatePositionsManually(true);
		
		EXPECT_EQ(UIEx(detached).GetChildrenBoundsSize(), preSize);
	}
}