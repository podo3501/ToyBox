#include "pch.h"
#include "UIComponentEx.h"
#include "UIComponent.h"
#include "../Utility.h"

namespace UIComponentEx
{
	Rectangle GetRectangle(const UIComponent* component) noexcept
	{
		const XMINT2& curPosition = component->GetPosition();
		const XMUINT2& curSize = GetSize(component);
		return Rectangle(curPosition.x, curPosition.y, curSize.x, curSize.y);
	}

	static inline const TransformContainer& GetTransformContainer(const UIComponent* component) noexcept { return component->GetTransformContainer(); }
	static inline TransformContainer& GetTransformContainer(UIComponent* component) noexcept { return component->GetTransformContainer(); }

	const string& GetName(const UIComponent* component) noexcept
	{
		return GetTransformContainer(component).GetName();
	}

	bool Rename(UIComponent* component, const string& name) noexcept
	{
		return GetTransformContainer(component).Rename(name);
	}

	optional<XMINT2> GetRelativePosition(const UIComponent* component) noexcept
	{
		return GetTransformContainer(component).GetRelativePosition();
	}

	bool SetRelativePosition(UIComponent* component, const XMINT2& relativePos) noexcept
	{
		return GetTransformContainer(component).SetRelativePosition(relativePos);
	}

	UIComponent* GetComponent(UIComponent* component, const string& name) noexcept
	{
		TransformContainer* transformContainer = GetTransformContainer(component).GetComponent(name);
		return transformContainer->GetComponent();
	}
}
