#pragma once
#include "../UIComponent.h"
#include "Toy/UserInterface/UIComponentLocator.h"
#include "Toy/UserInterface/Traversers.h"
#include "Toy/UserInterface/UIComponent/Traverser/BaseTraverser.h"
#include "Toy/UserInterface/UIComponent/Traverser/DerivedTraverser.h"
#include "Toy/UserInterface/UIComponent/Traverser/NameTraverser.h"

class UIComponent;

namespace UITraverser
{
	template<typename TraverserType, TraverserType* (*Getter)()>
	struct TraverserCaller {
		template<typename Ret, typename... Args, typename... CallArgs>
		static Ret Call(Ret(TraverserType::* fn)(Args...), CallArgs&&... args) noexcept {
			return (Getter()->*fn)(std::forward<CallArgs>(args)...);
		}
	};

	inline Traversers* GetTraversers() noexcept { return UIComponentLocator::GetService()->GetTraversers(); }
	inline BaseTraverser* GetBaseTraverser() noexcept { return GetTraversers()->GetBaseTraverser(); }
	inline DerivedTraverser* GetDerivedTraverser() noexcept { return GetTraversers()->GetDerivedTraverser(); }
	inline NameTraverser* GetNameTraverser() noexcept { return GetTraversers()->GetNameTraverser(); }

	using Base = TraverserCaller<BaseTraverser, GetBaseTraverser>;
	using Derived = TraverserCaller<DerivedTraverser, GetDerivedTraverser>;
	using Name = TraverserCaller<NameTraverser, GetNameTraverser>;

	inline unique_ptr<UIComponent> AttachComponent(UIComponent* parent,
		unique_ptr<UIComponent> child, const XMINT2& relativePos = {}) noexcept {
		return Name::Call(&NameTraverser::AttachComponent, parent, std::move(child), relativePos);
	}
	inline pair<unique_ptr<UIComponent>, UIComponent*> DetachComponent(UIComponent* c) noexcept {
		return Name::Call(&NameTraverser::DetachComponent, c);
	}
	inline UIComponent* FindComponent(UIComponent* c, const string& name) noexcept {
		return Name::Call(&NameTraverser::FindComponent, c, name);
	}
	inline UIComponent* FindRegionComponent(UIComponent* c, const string& region) noexcept {
		return Name::Call(&NameTraverser::FindRegionComponent, c, region);
	}
	inline XMUINT2 GetChildrenBoundsSize(UIComponent* c) noexcept {
		return Base::Call(&BaseTraverser::GetChildrenBoundsSize, c);
	}
	inline vector<UIComponent*> PickComponents(UIComponent* c, const XMINT2& pos) noexcept {
		return Base::Call(&BaseTraverser::PickComponents, c, pos);
	}
	inline vector<IMouseEventReceiver*> PickMouseReceivers(UIComponent* c, const XMINT2& pos) noexcept {
		return Base::Call(&BaseTraverser::PickMouseReceivers, c, pos);
	}
	inline bool Rename(UIComponent* c, const string& name) noexcept {
		return Name::Call(&NameTraverser::Rename, c, name);
	}
	inline bool RenameRegion(UIComponent* c, const string& region) noexcept {
		return Name::Call(&NameTraverser::RenameRegion, c, region);
	}
	//inline void PropagateRoot(UIComponent* c, UIComponent* root) noexcept {
	//	return Derived::Call(&DerivedTraverser::PropagateRoot, c, root);
	//}
	inline bool UpdatePositionsManually(UIComponent* c, bool isRoot = false) noexcept {
		return Derived::Call(&DerivedTraverser::UpdatePositionsManually, c, isRoot);
	}
	inline bool ChangeSize(UIComponent* c, const XMUINT2& size, bool isForce = false) noexcept {
		return Derived::Call(&DerivedTraverser::ChangeSize, c, size, isForce);
	}
	inline unique_ptr<UIComponent> Clone(UIComponent* c) noexcept {
		return Derived::Call(&DerivedTraverser::Clone, c);
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

	//void PropagateRoot(UIComponent* c) noexcept; //자신이 root일때 자신을 root라고 밑에 노드에게 전파
	bool ChangeSize(UIComponent* c, uint32_t x, uint32_t y, bool isForce = false) noexcept;
	bool ChangeSizeX(UIComponent* c, uint32_t v) noexcept;
	bool ChangeSizeX(UIComponent* c, const XMUINT2& s) noexcept;
	bool ChangeSizeY(UIComponent* c, uint32_t v) noexcept;
	bool ChangeSizeY(UIComponent* c, const XMUINT2& s) noexcept;
}