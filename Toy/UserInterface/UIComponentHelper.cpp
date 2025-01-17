#include "pch.h"
#include "UIComponentHelper.h"
#include "UIComponent.h"
#include "../Utility.h"

Rectangle GetRectangle(const UIComponent* component) noexcept
{
	const XMINT2& curPosition = component->GetPosition();
	const XMUINT2& curSize = component->GetSize();
	return Rectangle(curPosition.x, curPosition.y, curSize.x, curSize.y);
}