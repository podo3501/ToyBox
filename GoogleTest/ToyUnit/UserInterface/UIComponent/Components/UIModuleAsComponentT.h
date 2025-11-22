#pragma once
//#include "UserInterface/BaseModuleT.h"
#include "UserInterface/UIModuleT.h"
#include "Toy/UserInterface/UIComponent/Components/UIModuleAsComponent.h"

class UIModuleAsComponentT : public UIModuleT
{
protected:
	virtual void SetUp() override;

	unique_ptr<UIModuleAsComponent> m_component;
};

void UIModuleAsComponentT::SetUp()
{
	UIModuleT::SetUp();

	m_component = CreateComponent<UIModuleAsComponent>(m_uiModule);
}


