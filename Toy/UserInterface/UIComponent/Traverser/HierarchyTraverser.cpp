#include "pch.h"
#include "HierarchyTraverser.h"
#include "../UIComponent.h"

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

bool HierarchyTraverser::ForEachChildWithRegion(UIComponent* c, const string& parentRegion, function<bool(const string&, UIComponent*, bool)> Func) noexcept
{
	const auto Traverse = [&](UIComponent* component, const string& inheritedRegion, auto&& self_ref) -> bool {
		string currentRegion = component->GetRegion().empty() ? inheritedRegion : component->GetRegion();
		bool isNewRegion{ currentRegion != inheritedRegion };
		if (!isNewRegion && !component->GetRegion().empty()) // region이 같더라도, component가 region을 "명시적으로" 지정했다면 새 region으로 간주
			isNewRegion = true;
		ReturnIfFalse(Func(currentRegion, component, isNewRegion)); //새로운 region을 생성하려면 isNewRegion은 true

		const string& updatedRegion = component->GetRegion();
		const string nextRegion = updatedRegion.empty() ? currentRegion : updatedRegion;

		for (auto& child : component->GetChildren()) {
			if (child && !self_ref(child, nextRegion, self_ref))
				return false;
		}

		return true; };

	string curRegion = !parentRegion.empty() ? parentRegion : GetMyRegion(c);
	return Traverse(c, curRegion, Traverse);
}

void HierarchyTraverser::ForEachChildInSameRegion(UIComponent* c, function<void(UIComponent*)> Func) noexcept
{
	const auto Traverse = [&](UIComponent* component, const string& region, auto&& self_ref) -> void {
		const string& nodeRegion = component->GetRegion();
		if (!nodeRegion.empty() && nodeRegion != region)
			return;

		Func(component);

		for (auto& child : component->GetChildren()) {
			if (child)
				self_ref(child, region, self_ref);
		}};

	UIComponent* regionComponent = GetRegionRoot(c);
	Traverse(c, regionComponent->GetRegion(), Traverse);
}