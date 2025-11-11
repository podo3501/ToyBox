#pragma once
#include "../UIType.h"

class UIComponent;
class HierarchyTraverser
{
public:
	UIComponent* GetRoot(UIComponent* c) const noexcept;
	UIComponent* GetRegionRoot(UIComponent* c) const noexcept;
	UIComponent* GetParentRegionRoot(UIComponent* c) const noexcept;
	const string& GetMyRegion(UIComponent* c) const noexcept;

	void ForEachChildToRender(UIComponent* c, function<TraverseResult(UIComponent*)> Func) noexcept;
	void ForEachChildConst(UIComponent* c, const function<void(const UIComponent*)>& Func) const noexcept;
	void ForEachChildBool(UIComponent* c, function<TraverseResult(UIComponent*)> Func) noexcept;
	bool ForEachChildWithRegion(UIComponent* c, const string& parentRegion, function<bool(const string&, UIComponent*, bool)> Func) noexcept;
	void ForEachChildInSameRegion(UIComponent* c, function<void(UIComponent*)> Func) noexcept;

private:
	void ForEachRenderChildBFS(UIComponent* c, function<TraverseResult(UIComponent*)> Func) noexcept;
	void ForEachRenderChildDFS(UIComponent* c, function<TraverseResult(UIComponent*)> Func) noexcept;
};