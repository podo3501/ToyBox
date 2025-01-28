#include "pch.h"
#include "UIComponentEx.h"
#include "../Utility.h"

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
	// 루트를 찾기
	UIComponent* root = component->GetRoot();
	UIComponent* foundComponent = nullptr;

	// Traverse를 사용하여 트리 구조에서 이름을 찾기
	root->ForEachChild([&foundComponent, &name](UIComponent* comp) {
		if (comp->GetName() == name) {
			foundComponent = comp;
		}
		});

	return foundComponent;
}