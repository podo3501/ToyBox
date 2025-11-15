#include "pch.h"
#include "HierarchyTraverser.h"
#include "../UIComponent.h"

UIComponent* HierarchyTraverser::GetRoot(UIComponent* c) const noexcept
{
	while (auto parent = c->GetParent())
		c = parent;

	return c;
}

UIComponent* HierarchyTraverser::GetRegionRoot(UIComponent* c) const noexcept
{
	while (c->GetRegion().empty()) //값이 있으면 RegionRoot 이다.
	{
		auto parent = c->GetParent();
		if (!parent) break; //끝까지 올라가서 없으면 root 를 리턴
		c = parent;
	}

	return c;
}

UIComponent* HierarchyTraverser::GetParentRegionRoot(UIComponent* c) const noexcept
{
	auto parent = c->GetParent();
	if (!parent) return c;

	return GetRegionRoot(parent);
}

const string& HierarchyTraverser::GetMyRegion(UIComponent* c) const noexcept
{
	return GetRegionRoot(c)->GetRegion();
}

void HierarchyTraverser::ForEachChild(UIComponent* c, const function<void(UIComponent*)>& Func) noexcept
{
	Func(c);
	for (const auto& child : c->GetChildren())
		ForEachChild(child, Func);
}

void HierarchyTraverser::ForEachChildConst(UIComponent* c, const function<void(const UIComponent*)>& Func) const noexcept
{
	Func(c);
	for (const auto& child : c->GetChildren())
		ForEachChildConst(child, Func);
}

void HierarchyTraverser::ForEachChildBool(UIComponent* c, const function<TraverseResult(UIComponent*)>& Func) noexcept
{
	if (Func(c) == TraverseResult::Stop) return;

	for (auto& child : c->GetChildren())
		ForEachChildBool(child, Func);
}

void HierarchyTraverser::ForEachRenderChildBFS(UIComponent* c, const function<TraverseResult(UIComponent*)>& Func) noexcept
{
	queue<UIComponent*> cQueue;
	auto PushChild = [&cQueue](UIComponent* comp) { if (comp->HasStateFlag(StateFlag::Render)) cQueue.push(comp); };

	PushChild(c);

	while (!cQueue.empty())
	{
		UIComponent* current = cQueue.front();
		cQueue.pop();

		if (current->GetRenderSearchType() == RenderTraversal::DFS)
		{
			ForEachRenderChildDFS(current, Func);
			continue;
		}

		auto result = Func(current);
		Assert(result != TraverseResult::Stop); //Stop 리턴값이 와서는 안된다. 이건 매프레임 렌더링 되는 함수이기 때문에 속도가 중요하다.
		if (result == TraverseResult::ChildrenSkip) continue;

		for (auto child : current->GetChildren())
			PushChild(child);
	}
}

void HierarchyTraverser::ForEachRenderChildDFS(UIComponent* c, const function<TraverseResult(UIComponent*)>& Func) noexcept
{
	auto result = Func(c);
	Assert(result != TraverseResult::Stop); //Stop 리턴값이 와서는 안된다. 이건 매프레임 렌더링 되는 함수이기 때문에 속도가 중요하다.
	if (result == TraverseResult::ChildrenSkip) return;

	for (auto child : c->GetChildren())
		ForEachRenderChildDFS(child, Func);
}

bool HierarchyTraverser::ForEachChildPostUntilFail(UIComponent* c, 
	const function<bool(UIComponent*)>& Func) noexcept
{
	for (auto& child : c->GetChildren())
		if (child && !ForEachChildPostUntilFail(child, Func))
			return false;

	if (!Func(c))
		return false;

	return true;
}

void HierarchyTraverser::ForEachChildToRender(UIComponent* c, const function<TraverseResult(UIComponent*)>& Func) noexcept
{
	RenderTraversal traversal = c->GetRenderSearchType();
	if (traversal == RenderTraversal::BFS || traversal == RenderTraversal::Inherited)
		return ForEachRenderChildBFS(c, Func);

	return ForEachRenderChildDFS(c, Func);
}

bool HierarchyTraverser::ForEachChildWithRegion(UIComponent* c, const string& parentRegion, const function<bool(const string&, UIComponent*, bool)>& Func) noexcept
{
	const auto Traverse = [&](UIComponent* component, const string& inheritedRegion, auto&& self_ref) -> bool {
		string currentRegion = component->GetRegion().empty() ? inheritedRegion : component->GetRegion();
		bool isNewRegion{ currentRegion != inheritedRegion };
		if (!isNewRegion && !component->GetRegion().empty()) // region이 같더라도, component가 region을 "명시적으로" 지정했다면 새 region으로 간주
			isNewRegion = true;
		ReturnIfFalse(Func(currentRegion, component, isNewRegion)); //새로운 region을 생성하려면 isNewRegion은 true

		const string& updatedRegion = component->GetRegion();
		const string nextRegion = updatedRegion.empty() ? currentRegion : updatedRegion;

		for (auto& child : component->GetChildren())
			ReturnIfFalse(self_ref(child, nextRegion, self_ref));
		return true; 
		};

	string curRegion = !parentRegion.empty() ? parentRegion : GetMyRegion(c);
	return Traverse(c, curRegion, Traverse);
}

void HierarchyTraverser::ForEachChildInSameRegion(UIComponent* c, const function<void(UIComponent*)>& Func) noexcept
{
	const auto Traverse = [&](UIComponent* component, const string& region, auto&& self_ref) -> void {
		const string& nodeRegion = component->GetRegion();
		if (!nodeRegion.empty() && nodeRegion != region)
			return;

		Func(component);

		for (auto& child : component->GetChildren())
				self_ref(child, region, self_ref);
		};

	UIComponent* regionComponent = GetRegionRoot(c);
	Traverse(c, regionComponent->GetRegion(), Traverse);
}