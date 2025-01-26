#pragma once
#include "UIComponent.h"

class UIComponent;

namespace UIComponentEx
{
	Rectangle GetRectangle(const UIComponent* component) noexcept;

	inline const XMUINT2& GetSize(const UIComponent* component) noexcept {
		return component->GetLayout().GetSize();
	}
}
