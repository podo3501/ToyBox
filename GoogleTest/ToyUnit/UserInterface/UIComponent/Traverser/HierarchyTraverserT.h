#pragma once
#include "UserInterface/UIModul2T.h"
#include "Toy/UserInterface/UIComponentLocator.h"
#include "Toy/UserInterface/UIComponent/Traverser/HierarchyTraverser.h"

class HierarchyTraverserT : public UIModul2T
{
protected:
	virtual void SetUp() override;
	virtual void TearDown() override {};

	HierarchyTraverser m_traverser;
};

void HierarchyTraverserT::SetUp()
{
	UIModul2T::SetUp();
}
