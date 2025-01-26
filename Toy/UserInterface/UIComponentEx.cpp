#include "pch.h"
#include "UIComponentEx.h"
#include "UIComponent.h"
#include "../Utility.h"

static inline const TransformContainer& GetTransformContainer(const UIComponent* component) noexcept { return component->GetTransformContainer(); }
static inline TransformContainer& GetTransformContainer(UIComponent* component) noexcept { return component->GetTransformContainer(); }

namespace UIComponentEx
{
	Rectangle GetRectangle(const UIComponent* component) noexcept
	{
		const XMINT2& curPosition = GetTransformContainer(component).GetPosition();
		const XMUINT2& curSize = GetSize(component);
		return Rectangle(curPosition.x, curPosition.y, curSize.x, curSize.y);
	}

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

	const XMUINT2& GetSize(const UIComponent* component) noexcept 
	{
		return GetTransformContainer(component).GetLayout().GetSize();
	}

	void ChangeOrigin(UIComponent* component, Origin origin) noexcept
	{
		return GetTransformContainer(component).ChangeOrigin(origin);
	}

	//void SetSize(UIComponent* component, const XMUINT2& size) noexcept
	//{
	//	return GetTransformContainer(component).SetSize(size);
	//}
	
	void SetLayout(UIComponent* component, const UILayout& layout) noexcept
	{
		GetTransformContainer(component).SetLayout(layout);
	}
	
	void GetComponents(UIComponent* component, const XMINT2& pos, vector<UIComponent*>& outList) noexcept
	{
		GetTransformContainer(component).GetComponents(pos, outList);
	}

	XMINT2 GetPosition(UIComponent* component) noexcept
	{
		return GetTransformContainer(component).GetPosition();
	}

	XMUINT2 GetTotalChildSize(UIComponent* component) noexcept
	{
		return GetTransformContainer(component).GetTotalChildSize();
	}
}

namespace UIComponentLayout
{
	void SetSize(UIComponent* component, const XMUINT2& size) noexcept
	{
		TransformContainer& transformContainer = GetTransformContainer(component);
		UILayout& layout = transformContainer.GetLayout();
		layout.Set(size);
		transformContainer.MarkDirty();
	}
}