#pragma once
#include "Toy/UserInterface/UIComponent/Traverser/HierarchyTraverser.h"

class HierarchyTraverserT : public testing::Test
{
protected:
	virtual void SetUp() override {};
	virtual void TearDown() override {};

	HierarchyTraverser m_traverser;
};