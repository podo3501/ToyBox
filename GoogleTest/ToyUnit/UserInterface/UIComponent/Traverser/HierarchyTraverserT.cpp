#include "pch.h"
#include "HierarchyTraverserT.h"
#include "../MockComponent.h"
#include "Toy/UserInterface/UIComponent/Traverser/UITraverser.h"

using namespace UITraverser;

namespace UserInterfaceT::UIComponentT::TraverserT
{
	TEST_F(HierarchyTraverserT, GetRegionRoot_Empty) //자기 Region name이 존재 하지 않을때
	{
		auto [mOwner, main] = CreateMockComponent<MockComponent>();
		auto component = AttachMockComponent<MockComponent>(main);

		EXPECT_EQ(main, m_traverser.GetRegionRoot(component));
	}

	TEST_F(HierarchyTraverserT, GetRegionRoot_Exist) //자기 Region name이 존재 할때
	{
		auto [mOwner, main] = CreateMockComponent<MockComponent>();
		auto component = AttachMockComponent<MockComponent>(main);
		RenameRegion(main, "region");
		
		EXPECT_EQ(main, m_traverser.GetRegionRoot(component));
	}
}