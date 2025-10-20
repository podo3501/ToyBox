#include "pch.h"
#include "Fixture/FixtureSuite.h"
#include "Mocks/Stubs/UIComponentStub.h"
#include "Mocks/MockUtils.h"

class MockComponent : public UIComponentStub
{};

namespace UserInterface
{
	TEST_F(UIComponentT, FindComponent)
	{
		auto [parent, child] = CreateTwoLevelComponents<MockComponent>(m_main);

		UIEx(parent).RenameRegion("Region1");
		UIEx(parent).Rename("Parent"); 
		UIEx(child).Rename("Child");

		//False인 것은 찾는 Component와 다른 Region이기 때문에 찾을 수 없다.
		EXPECT_FALSE(UIEx(m_main).FindComponent("Parent"));
		EXPECT_FALSE(UIEx(m_main).FindComponent("Child"));

		EXPECT_FALSE(UIEx(parent).FindComponent("Main"));
		EXPECT_TRUE(UIEx(parent).FindComponent("Child"));

		EXPECT_FALSE(UIEx(child).FindComponent("Main"));
		EXPECT_TRUE(UIEx(child).FindComponent("Parent"));
	}

	TEST_F(UIComponentT, FindRegionComponent)
	{
		auto [parent, child] = CreateTwoLevelComponents<MockComponent>(m_main);
		UIEx(parent).RenameRegion("Region1");
		UIEx(child).RenameRegion("Region2");

		//Region이 있는 Component는 노드 밑으로만 찾고 위로는 찾지 않는다.
		EXPECT_EQ(UIEx(m_main).FindRegionComponent("Region1"), parent);
		EXPECT_FALSE(UIEx(m_main).FindRegionComponent("Region2"));

		EXPECT_EQ(UIEx(parent).FindRegionComponent("Region2"), child);
		EXPECT_FALSE(UIEx(child).FindRegionComponent("Region1"));
	}

	TEST_F(UIComponentT, FindRenderComponents)
	{
		auto [parent, child] = CreateTwoLevelComponents<MockComponent>(m_main);

		vector<UIComponent*> compAtParent = UIEx(m_main).FindRenderComponents(parent->GetPosition());
		EXPECT_EQ(compAtParent[0], m_main);
		EXPECT_EQ(compAtParent[1], parent);

		vector<UIComponent*> compAtChild = UIEx(m_main).FindRenderComponents(child->GetPosition());
		EXPECT_EQ(compAtChild[2], child);
	}

	TEST_F(UIComponentT, LockedSize)
	{
		auto compPtr = CreateOneLevelComponent<MockComponent>(m_main);

		XMUINT2 curSize{ compPtr->GetSize() }, newSize{128, 128};
		compPtr->SetStateFlag(StateFlag::X_SizeLocked, true);
		compPtr->SetStateFlag(StateFlag::Y_SizeLocked, true);
		compPtr->ChangeSize(newSize);

		EXPECT_EQ(compPtr->GetSize(), curSize);
	}
}