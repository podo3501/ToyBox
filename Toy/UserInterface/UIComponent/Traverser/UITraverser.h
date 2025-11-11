#pragma once
#include "Toy/UserInterface/UIComponentLocator.h"
#include "Toy/UserInterface/UIComponent/Traverser/BaseTraverser.h"
#include "Toy/UserInterface/UIComponent/Traverser/NameTraverser.h"

class UIComponent;

template <typename Fn, typename... Args>
decltype(auto) BaseCall(Fn&& fn, Args&&... args) noexcept
{
	auto traverser = UIComponentLocator::GetService()->GetBaseTraverser();
	return std::invoke(std::forward<Fn>(fn), traverser, std::forward<Args>(args)...);
}

template <typename Fn, typename... Args>
decltype(auto) NameCall(Fn&& fn, Args&&... args) noexcept
{
	auto traverser = UIComponentLocator::GetService()->GetNameTraverser();
	return std::invoke(std::forward<Fn>(fn), traverser, std::forward<Args>(args)...);
}

namespace UITraverser
{
	inline unique_ptr<UIComponent> AttachComponent(UIComponent* parent,
		unique_ptr<UIComponent> child, const XMINT2& relativePos = {}) noexcept {
		return NameCall(&NameTraverser::AttachComponent, parent, move(child), relativePos);
	}
	inline pair<unique_ptr<UIComponent>, UIComponent*> DetachComponent(UIComponent* c) noexcept {
		return NameCall(&NameTraverser::DetachComponent, c);
	}
	inline UIComponent* FindComponent(UIComponent* c, const string& name) noexcept {
		return NameCall(&NameTraverser::FindComponent, c, name);
	}
	inline UIComponent* FindRegionComponent(UIComponent* c, const string& region) noexcept {
		return NameCall(&NameTraverser::FindRegionComponent, c, region);
	}
	inline XMUINT2 GetChildrenBoundsSize(UIComponent* c) noexcept {
		return BaseCall(&BaseTraverser::GetChildrenBoundsSize, c);
	}
	inline vector<UIComponent*> PickComponents(UIComponent* c, const XMINT2& pos) noexcept {
		return BaseCall(&BaseTraverser::PickComponents, c, pos);
	}
	inline vector<IMouseEventReceiver*> PickMouseReceivers(UIComponent* c, const XMINT2& pos) noexcept {
		return BaseCall(&BaseTraverser::PickMouseReceivers, c, pos);
	}
	inline bool Rename(UIComponent* c, const string& name) noexcept {
		return NameCall(&NameTraverser::Rename, c, name);
	}
	inline bool RenameRegion(UIComponent* c, const string& region) noexcept {
		return NameCall(&NameTraverser::RenameRegion, c, region);
	}

	unique_ptr<UIComponent> AttachComponent(UIComponent* c, const string& region, const string& name,
		unique_ptr<UIComponent> child, const XMINT2& relativePos = {}) noexcept;
	pair<unique_ptr<UIComponent>, UIComponent*> DetachComponent(UIComponent* c, 
		const string& region, const string& name) noexcept;

	UIComponent* FindComponent(UIComponent* c, const string& region, const string& name) noexcept;
}