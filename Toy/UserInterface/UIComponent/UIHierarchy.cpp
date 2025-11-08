#include "pch.h"
#include "UIHierarchy.h"
#include "UIComponent.h"
#include "UIComponentEx.h"

//const vector<unique_ptr<UIComponent>>& UIHierarchy<UIComponent>::GetChildren() const noexcept
//{ 
//	return m_children; 
//}

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
	while (current->GetRegion().empty()) //값이 있으면 RegionRoot 이다.
	{
		if (!current->m_parent) break; //끝까지 올라가서 없으면 root 를 리턴
		current = current->m_parent;
	}

	return current;
}

UIComponent* UIHierarchy<UIComponent>::GetParentRegionRoot() const noexcept
{
	if (m_parent == nullptr) return GetThis();
	return m_parent->GetRegionRoot();
}

const string& UIHierarchy<UIComponent>::GetMyRegion() const noexcept
{
	return GetRegionRoot()->GetRegion();
}

void UIHierarchy<UIComponent>::ForEachChildBool(function<TraverseResult(UIComponent*)> Func) noexcept
{
	auto result = Func(GetThis());
	if (result == TraverseResult::Stop)
		return;

	for (auto& child : m_children)
	{
		if (child)
			child->ForEachChildBool(Func);
	}
}

void UIHierarchy<UIComponent>::ForEachRenderChildBFS(function<TraverseResult(UIComponent*)> Func) noexcept
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

		auto result = Func(current);
		Assert(result != TraverseResult::Stop); //Stop 리턴값이 와서는 안된다. 이건 매프레임 렌더링 되는 함수이기 때문에 속도가 중요하다.
		if (result == TraverseResult::ChildrenSkip) continue;

		for (const auto& child : current->m_children)
		{
			if (!child) continue;
			PushChild(child.get());
		}
	}
}

template<typename T>
void UIHierarchy<T>::ForEachRenderChildDFS(function<TraverseResult(UIComponent*)> Func) noexcept
{
	UIComponent* current = GetThis();
	auto result = Func(current);
	Assert(result != TraverseResult::Stop); //Stop 리턴값이 와서는 안된다. 이건 매프레임 렌더링 되는 함수이기 때문에 속도가 중요하다.
	if (result == TraverseResult::ChildrenSkip) return;

	for (auto& child : m_children)
	{
		if (!child) continue;
		child->ForEachRenderChildDFS(Func);
	}
}

void UIHierarchy<UIComponent>::ForEachChildToRender(function<TraverseResult(UIComponent*)> Func) noexcept
{
	UIComponent* current = GetThis();
	RenderTraversal traversal = current->GetRenderSearchType();
	if(traversal == RenderTraversal::BFS || traversal == RenderTraversal::Inherited )
		return ForEachRenderChildBFS(Func);

	return ForEachRenderChildDFS(Func);
}

bool UIHierarchy<UIComponent>::ForEachChildWithRegion(const string& parentRegion, function<bool(const string&, UIComponent*, bool)> Func) noexcept
{
	const auto Traverse = [&](UIHierarchy<UIComponent>* node, const string& inheritedRegion, auto&& self_ref) -> bool {
		UIComponent* component = static_cast<UIComponent*>(node);
		string currentRegion = component->GetRegion().empty() ? inheritedRegion : component->GetRegion();
		bool isNewRegion{ currentRegion != inheritedRegion };
		if (!isNewRegion && !component->GetRegion().empty()) // region이 같더라도, component가 region을 "명시적으로" 지정했다면 새 region으로 간주
			isNewRegion = true;
		ReturnIfFalse(Func(currentRegion, component, isNewRegion)); //새로운 region을 생성하려면 isNewRegion은 true

		const string& updatedRegion = component->GetRegion();
		const string nextRegion = updatedRegion.empty() ? currentRegion : updatedRegion;

		for (auto& child : node->m_children) {
			if (child && !self_ref(child.get(), nextRegion, self_ref))
				return false;
		}

		return true; };

	string curRegion = !parentRegion.empty() ? parentRegion : GetMyRegion();
	return Traverse(GetThis(), curRegion, Traverse);
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