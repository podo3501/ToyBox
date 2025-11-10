#pragma once
#include <memory>
#include <string>
#include <DirectXMath.h>
#include "NameTraverser.h"
#include "Toy/UserInterface/UIComponentLocator.h"

class UIComponent;

template <typename Fn, typename... Args>
decltype(auto) AttachCall(Fn&& fn, Args&&... args) noexcept
{
	auto traverser = UIComponentLocator::GetService()->GetAttachmentTraverser();
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
	inline std::unique_ptr<UIComponent> AttachComponent(UIComponent* parent,
		std::unique_ptr<UIComponent> child, const DirectX::XMINT2& relativePos = {}) noexcept {
		return AttachCall(&AttachmentTraverser::AttachComponent, parent, move(child), relativePos);
	}
	inline UIComponent* FindComponent(UIComponent* c, const string& name) noexcept {
		return NameCall(&NameTraverser::FindComponent, c, name);
	}
	inline bool Rename(UIComponent* c, const std::string& name) noexcept {
		return NameCall(&NameTraverser::Rename, c, name);
	}
	inline bool RenameRegion(UIComponent* c, const std::string& region) noexcept {
		return NameCall(&NameTraverser::RenameRegion, c, region);
	}
}