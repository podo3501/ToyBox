#pragma once
#include "UserInterface/BaseModuleT.h"
#include "Toy/UserInterface/UIComponent/Components/UIModuleAsComponent.h"

class UIModuleAsComponentT : public BaseModuleT
{
protected:
	virtual void SetUp() override;
	virtual void TearDown() override {};

	unique_ptr<UIModuleAsComponent> m_component;
};

void UIModuleAsComponentT::SetUp()
{
	BaseModuleT::SetUp();

	m_component = CreateComponent<UIModuleAsComponent>(m_uiModule->Clone());
}


