#pragma once
#include "UserInterface/UIModuleT.h"
#include "Toy/UserInterface/UIComponentLocator.h"
#include "Toy/UserInterface/UIComponent/Traverser/NameTraverser.h"
#include "Toy/UserInterface/UIComponent/Components/Panel.h"

class NameTraverserT : public UIModuleT
{
protected:
	virtual void SetUp() override;

	NameTraverser m_traverser;
};

void NameTraverserT::SetUp()
{
	UIModuleT::SetUp();
}