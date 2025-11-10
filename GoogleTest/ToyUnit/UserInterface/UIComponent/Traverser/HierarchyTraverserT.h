#pragma once
#include "Toy/UserInterface/UIComponentLocator.h"
#include "Toy/UserInterface/UIComponent/Traverser/HierarchyTraverser.h"

class HierarchyTraverserT : public testing::Test
{
protected:
	virtual void SetUp() override;
	virtual void TearDown() override {};

	unique_ptr<UIComponentManager> m_componentManager;
	HierarchyTraverser m_traverser;
};

void HierarchyTraverserT::SetUp()
{
	m_componentManager = make_unique<UIComponentManager>();
	Locator<UIComponentManager>::Provide(m_componentManager.get());
}
