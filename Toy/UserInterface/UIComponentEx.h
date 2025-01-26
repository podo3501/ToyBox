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
	const XMUINT2& GetSize(const UIComponent* component) noexcept;
	void ChangeOrigin(UIComponent* component, Origin origin) noexcept;
	//void SetSize(UIComponent* component, const XMUINT2& size) noexcept;
	void SetLayout(UIComponent* component, const UILayout& layout) noexcept;
	void GetComponents(UIComponent* component, const XMINT2& pos, vector<UIComponent*>& outList) noexcept;
	XMINT2 GetPosition(UIComponent* component) noexcept;
	XMUINT2 GetTotalChildSize(UIComponent* component) noexcept;
}

namespace UIComponentLayout
{
	void SetSize(UIComponent* component, const XMUINT2& size) noexcept;
}
