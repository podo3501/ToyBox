#pragma once
#include "Toy/UserInterface/UIComponentLocator.h"
#include "Toy/UserInterface/UIComponent/Traverser/BaseTraverser.h"
#include "Toy/UserInterface/UIComponent/Traverser/DerivedTraverser.h"
#include "Toy/UserInterface/UIComponent/Traverser/NameTraverser.h"
#include "../UIComponent.h" //?!? ComponentCast 이것때문에 여기 있는데 이 인클루드 제거해야 한다.

class UIComponent;

namespace UITraverser
{
	inline BaseTraverser* GetBaseTraverser() noexcept { return UIComponentLocator::GetService()->GetBaseTraverser(); }
	template <typename Fn, typename... Args>
	decltype(auto) BaseCall(Fn&& fn, Args&&... args) noexcept {
		return std::invoke(std::forward<Fn>(fn), GetBaseTraverser(), std::forward<Args>(args)...);
	}

	inline DerivedTraverser* GetDerivedTraverser() noexcept { return UIComponentLocator::GetService()->GetDerivedTraverser(); }
	template <typename Fn, typename... Args>
	decltype(auto) DerivedCall(Fn&& fn, Args&&... args) noexcept {
		return std::invoke(std::forward<Fn>(fn), GetDerivedTraverser(), std::forward<Args>(args)...);
	}

	inline NameTraverser* GetNameTraverser() noexcept { return UIComponentLocator::GetService()->GetNameTraverser(); }
	template <typename Fn, typename... Args>
	decltype(auto) NameCall(Fn&& fn, Args&&... args) noexcept {
		return std::invoke(std::forward<Fn>(fn), GetNameTraverser(), std::forward<Args>(args)...);
	}

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
	inline void Render(UIComponent* c, ITextureRender* render) noexcept {
		return DerivedCall(&DerivedTraverser::Render, c, render);
	}
	inline bool BindTextureSourceInfo(UIComponent* c, TextureResourceBinder* resBinder, ITextureController* texController) noexcept {
		return DerivedCall(&DerivedTraverser::BindTextureSourceInfo, c, resBinder, texController);
	}
	inline void PropagateRoot(UIComponent* c, UIComponent* root) noexcept {
		return DerivedCall(&DerivedTraverser::PropagateRoot, c, root);
	}
	inline bool EnableToolMode(UIComponent* c, bool enable) noexcept {
		return DerivedCall(&DerivedTraverser::EnableToolMode, c, enable);
	}
	
	unique_ptr<UIComponent> AttachComponent(UIComponent* c, const string& region, const string& name,
		unique_ptr<UIComponent> child, const XMINT2& relativePos = {}) noexcept;
	pair<unique_ptr<UIComponent>, UIComponent*> DetachComponent(UIComponent* c, 
		const string& region, const string& name) noexcept;

	UIComponent* FindComponent(UIComponent* c, const string& region, const string& name) noexcept;

	template<typename T>
	T FindComponent(UIComponent* c, const string& name) noexcept
	{
		UIComponent* find = FindComponent(c, name);
		return ComponentCast<T>(find);
	}

	void PropagateRoot(UIComponent* c) noexcept; //자신이 root일때 자신을 root라고 밑에 노드에게 전파
	void Render(IComponent* c, ITextureRender* render) noexcept;
}