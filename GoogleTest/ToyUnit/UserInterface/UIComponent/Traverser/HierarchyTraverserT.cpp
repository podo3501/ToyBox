#include "pch.h"
#include "HierarchyTraverserT.h"
#include "../MockComponent.h"
#include "Toy/UserInterface/UIComponent/Components/Panel.h"
#include "Toy/UserInterface/UIModul2.h"
#include "Toy/UserInterface/UIComponent/Traverser/UITraverser.h"

using namespace UITraverser;

namespace UserInterfaceT::UIComponentT::TraverserT
{
	TEST_F(HierarchyTraverserT, GetRegionRoot_Empty) //자기 Region name이 존재 하지 않을때
	{
		auto component = AttachMockComponenT<MockComponent>(m_main);
		EXPECT_EQ(m_main, m_traverser.GetRegionRoot(component));
	}

	TEST_F(HierarchyTraverserT, GetRegionRoot_Exist) //자기 Region name이 존재 할때
	{
		auto component = AttachMockComponenT<MockComponent>(m_main);
		RenameRegion(m_main, "region");
		
		EXPECT_EQ(m_main, m_traverser.GetRegionRoot(component));
	}
}