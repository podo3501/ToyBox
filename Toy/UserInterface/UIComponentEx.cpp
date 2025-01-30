#include "pch.h"
#include "UIComponentEx.h"
#include "../Utility.h"

unique_ptr<UIComponent> UIComponentEx::Clone(const UIComponent* component)
{
	unique_ptr<UIComponent> clone = component->Clone();
	return clone;
}

Rectangle UIComponentEx::GetRectangle(const UIComponent* component) noexcept
{
	const XMINT2& curPosition = component->GetPosition();
	const XMUINT2& curSize = GetSize(component);
	return Rectangle(curPosition.x, curPosition.y, curSize.x, curSize.y);
}

const XMUINT2& UIComponentEx::GetSize(const UIComponent* component) noexcept
{
	return component->GetLayout().GetSize();
}

UIComponent* UIComponentEx::GetComponent(UIComponent* component, const string& name) noexcept
{
	UIComponent* root = component->GetRoot();
	UIComponent* foundComponent = nullptr;

	root->ForEachChild([&foundComponent, &name](UIComponent* comp) {
		if (comp->GetName() == name) {
			foundComponent = comp;
		}
		});

	return foundComponent;
}