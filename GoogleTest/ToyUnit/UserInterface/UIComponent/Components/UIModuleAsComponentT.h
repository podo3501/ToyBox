#pragma once
//#include "UserInterface/BaseModuleT.h"
#include "UserInterface/UIModul2T.h"
#include "Toy/UserInterface/UIComponent/Components/UIModuleAsComponent.h"

class UIModuleAsComponentT : public UIModul2T
{
protected:
	virtual void SetUp() override;
	virtual void TearDown() override {};

	unique_ptr<UIModuleAsComponent> m_component;
};

void UIModuleAsComponentT::SetUp()
{
	UIModul2T::SetUp();

	m_component = CreateComponent<UIModuleAsComponent>(m_uiModule);
}


