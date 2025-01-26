#include "pch.h"
#include "UIComponentEx.h"
#include "../Utility.h"

namespace UIComponentEx
{
	Rectangle GetRectangle(const UIComponent* component) noexcept
	{
		const XMINT2& curPosition = component->GetPosition();
		const XMUINT2& curSize = GetSize(component);
		return Rectangle(curPosition.x, curPosition.y, curSize.x, curSize.y);
	}
}
