#pragma once
#include "UIComponent.h"

class UIComponent;

namespace UIComponentEx
{
	Rectangle GetRectangle(const UIComponent* component) noexcept;

	const string& GetName(const UIComponent* component) noexcept;
	bool Rename(UIComponent* component, const string& name) noexcept;
	optional<XMINT2> GetRelativePosition(const UIComponent* component) noexcept;
	bool SetRelativePosition(UIComponent* component, const XMINT2& relativePos) noexcept;
	UIComponent* GetComponent(UIComponent* component, const string& name) noexcept;
	inline const XMUINT2& GetSize(const UIComponent* component) noexcept {
		return component->GetLayout().GetSize();
	}
}
