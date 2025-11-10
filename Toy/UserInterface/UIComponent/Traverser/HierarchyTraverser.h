#pragma once

class UIComponent;
class HierarchyTraverser
{
public:
	UIComponent* GetRegionRoot(UIComponent* c) const noexcept;
	UIComponent* GetParentRegionRoot(UIComponent* c) const noexcept;
	const string& GetMyRegion(UIComponent* c) const noexcept;

	bool ForEachChildWithRegion(UIComponent* c, const string& parentRegion, function<bool(const string&, UIComponent*, bool)> Func) noexcept;
	void ForEachChildInSameRegion(UIComponent* c, function<void(UIComponent*)> Func) noexcept;
};
