#pragma once
#include "Toy/UserInterface/UIComponent/Traverser/BaseTraverser.h"

class BaseTraverserT : public testing::Test
{
protected:
	virtual void SetUp() override {};
	virtual void TearDown() override {};

	BaseTraverser m_traverser;
};