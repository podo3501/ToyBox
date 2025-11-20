#include "pch.h"
#include "UITraverser.h"

namespace UITraverser
{
	unique_ptr<UIComponent> UITraverser::AttachComponent(UIComponent* c, const string& region, 
	const string& name, unique_ptr<UIComponent> child, const XMINT2& relativePos) noexcept
	{
		UIComponent* find = FindComponent(c, region, name);
		if (!find) return child;

		return AttachComponent(find, move(child), relativePos);
	}

	pair<unique_ptr<UIComponent>, UIComponent*> DetachComponent(UIComponent* c,
		const string& region, const string& name) noexcept
	{
		UIComponent* find = FindComponent(c, region, name);
		if (!find) return {};

		return DetachComponent(find);
	}

	UIComponent* FindComponent(UIComponent* c, const string& region, const string& name) noexcept
	{
		UIComponent* component = FindRegionComponent(c, region);
		if (component == nullptr) return nullptr;

		return FindComponent(component, name);
	}

	void PropagateRoot(UIComponent* c) noexcept { PropagateRoot(c, c); }
	bool ChangeSize(UIComponent* c, uint32_t x, uint32_t y, bool isForce) noexcept { return ChangeSize(c, { x, y }, isForce); }
	bool ChangeSizeX(UIComponent* c, uint32_t v) noexcept { return ChangeSize(c, { v, c->GetSize().y }); }
	bool ChangeSizeX(UIComponent* c, const XMUINT2& s) noexcept { return ChangeSizeX(c, s.x); }
	bool ChangeSizeY(UIComponent* c, uint32_t v) noexcept { return ChangeSize(c, { c->GetSize().x, v }); }
	bool ChangeSizeY(UIComponent* c, const XMUINT2& s) noexcept { return ChangeSizeY(c, s.y); }
}
