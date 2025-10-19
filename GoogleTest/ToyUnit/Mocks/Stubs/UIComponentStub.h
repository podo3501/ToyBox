#pragma once
#include "Toy/UserInterface/UIComponent/UIComponent.h"

class UIComponentStub : public UIComponent
{
public:
	virtual ComponentID GetTypeID() const noexcept { return UIComponent::GetTypeStatic(); }

protected:
	virtual unique_ptr<UIComponent> CreateClone() const { return nullptr; }
};
