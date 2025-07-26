#include "pch.h"
#include "UIHierarchy.h"
#include "UIComponent.h"
#include "UIComponentEx.h"
#include "Utility.h"

UIComponent* UIHierarchy<UIComponent>::GetThis() const noexcept
{
	UIHierarchy<UIComponent>* hierarchy = const_cast<UIHierarchy<UIComponent>*>(this);
	return static_cast<UIComponent*>(hierarchy);
}

UIComponent* UIHierarchy<UIComponent>::GetRoot() const noexcept
{
	UIComponent* current = GetThis();
	while (current->m_parent != nullptr)
		current = current->m_parent;

	return current;
}

UIComponent* UIHierarchy<UIComponent>::GetRegionRoot() const noexcept
{
	UIComponent* current = GetThis();
	while (current->GetRegion().empty())
	{
		if (!current->m_parent) break;
		current = current->m_parent;
	}

	return current;
}

UIComponent* UIHierarchy<UIComponent>::GetParentRegionRoot() const noexcept //?!? 자기껀 검사 안하고 부모로 올라가는데 그렇다면 이 함수가 쓰여야 할까?
{
	if (m_parent == nullptr) return GetThis();
	return m_parent->GetRegionRoot();
}

void UIHierarchy<UIComponent>::ForEachChildBool(function<CResult(UIComponent*)> Func) noexcept
{
	if (Func(GetThis()) == CResult::SkipChildren)
		return;

	for (auto& child : m_children)
	{
		if (child)
			child->ForEachChildBool(Func);
	}
}

void UIHierarchy<UIComponent>::ForEachRenderChildBFS(function<void(UIComponent*)> Func) noexcept
{
	queue<UIComponent*> cQueue;
	auto PushChild = [&cQueue](UIComponent* c) { if (c->HasStateFlag(StateFlag::Render)) cQueue.push(c); };

	PushChild(GetThis());

	while (!cQueue.empty())
	{
		UIComponent* current = cQueue.front();
		cQueue.pop();

		if (current->GetRenderSearchType() == RenderTraversal::DFS)
		{
			current->ForEachRenderChildDFS(Func);
			continue;
		}

		Func(current);
		if (current->HasStateFlag(StateFlag::RenderTexture)) continue;

		for (const auto& child : current->m_children)
		{
			if (!child) continue;
			PushChild(child.get());
		}
	}
}

template<typename T>
void UIHierarchy<T>::ForEachRenderChildDFS(function<void(UIComponent*)> Func) noexcept
{
	UIComponent* current = GetThis();
	Func(current);
	if (current->HasStateFlag(StateFlag::RenderTexture)) return;

	for (auto& child : m_children)
	{
		if (!child) continue;
		child->ForEachRenderChildDFS(Func);
	}
}

void UIHierarchy<UIComponent>::ForEachChildToRender(function<void(UIComponent*)> Func) noexcept
{
	UIComponent* current = GetThis();
	RenderTraversal traversal = current->GetRenderSearchType();
	if(traversal == RenderTraversal::BFS || traversal == RenderTraversal::Inherited )
		return ForEachRenderChildBFS(Func);

	return ForEachRenderChildDFS(Func);
}

void UIHierarchy<UIComponent>::ForEachChildWithRegion(function<void(const string&, UIComponent*)> Func) noexcept
{
	const auto Traverse = [&](UIHierarchy<UIComponent>* node, const string& region, auto&& self_ref) -> void {
		UIComponent* component = static_cast<UIComponent*>(node);
		string currentRegion = !component->GetRegion().empty() ? component->GetRegion() : region;
		Func(currentRegion, component);

		string childRegion{ region };
		const string& maybeChangedRegion = component->GetRegion();
		if (!maybeChangedRegion.empty() && maybeChangedRegion != region)
			childRegion = maybeChangedRegion;	//region이 Func()안에서 새로 생성되면서 바뀔수 있으므로 GetRegion 해서 현재 region이 다음 노드에 적용되도록 한다.

		for (auto& child : node->m_children) {
			if (child)
				self_ref(child.get(), childRegion, self_ref);	
		}};

	UIComponent* regionComponent = GetParentRegionRoot();
	Traverse(GetThis(), regionComponent->GetRegion(), Traverse);
}

void UIHierarchy<UIComponent>::ForEachChildInSameRegion(function<void(UIComponent*)> Func) noexcept
{
	const auto Traverse = [&](UIHierarchy<UIComponent>* node, const string& region, auto&& self_ref) -> void {
		UIComponent* component = static_cast<UIComponent*>(node);

		const string& nodeRegion = component->GetRegion();
		if (!nodeRegion.empty() && nodeRegion != region)
			return;

		Func(component);

		for (auto& child : node->m_children) {
			if (child)
				self_ref(child.get(), region, self_ref);
		}};

	UIComponent* regionComponent = GetRegionRoot();
	Traverse(GetThis(), regionComponent->GetRegion(), Traverse);
}