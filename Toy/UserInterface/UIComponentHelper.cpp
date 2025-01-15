#include "pch.h"
#include "UIComponentHelper.h"
#include "UIComponent.h"
#include "../Utility.h"

bool AddComponentFromScreenPos(
	UIComponent* addable, 
	unique_ptr<UIComponent>&& component, 
	const XMINT2& pos) noexcept
{
	const XMINT2& comPos = addable->GetPosition();
	XMINT2 relativePos = pos - comPos;

	return addable->AttachComponent(move(component), relativePos);
}

Rectangle GetRectangle(const UIComponent* component) noexcept
{
	const XMINT2& curPosition = component->GetPosition();
	const XMUINT2& curSize = component->GetSize();
	return Rectangle(curPosition.x, curPosition.y, curSize.x, curSize.y);
}