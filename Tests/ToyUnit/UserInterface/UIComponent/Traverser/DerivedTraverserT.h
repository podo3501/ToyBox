#pragma once
#include "Toy/UserInterface/UIComponent/Traverser/DerivedTraverser.h"

class DerivedTraverserT : public testing::Test
{
protected:
	virtual void SetUp() override {};
	virtual void TearDown() override {};

	DerivedTraverser m_traverser;
};