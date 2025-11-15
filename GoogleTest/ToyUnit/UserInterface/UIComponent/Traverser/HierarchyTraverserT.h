#pragma once
#include "UserInterface/UIModuleT.h"
#include "Toy/UserInterface/UIComponentLocator.h"
#include "Toy/UserInterface/UIComponent/Traverser/HierarchyTraverser.h"

class HierarchyTraverserT : public UIModuleT
{
protected:
	virtual void SetUp() override;
	virtual void TearDown() override {};

	HierarchyTraverser m_traverser;
};

void HierarchyTraverserT::SetUp()
{
	UIModuleT::SetUp();
}
