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

	m_component = CreateComponent<UIModuleAsComponent>(move(m_uiModule)); //?!? module에 clone 함수가 있다면 멤버함수를 이동하지 않아도 될것이다.
}


