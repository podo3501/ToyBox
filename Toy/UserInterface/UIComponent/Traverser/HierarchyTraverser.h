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

	void ForEachChild(UIComponent* c, const function<void(UIComponent*)>& Func) noexcept; //기본 foreach
	bool ForEachChildPostUntilFail(UIComponent* c, const function<bool(UIComponent*)>& Func) noexcept; //후위 순회로 리턴값 반환 재귀(binder에사용)
	void ForEachChildToRender(UIComponent* c, const function<TraverseResult(UIComponent*)>& Func) noexcept;
	void ForEachChildConst(UIComponent* c, const function<void(const UIComponent*)>& Func) const noexcept; //읽기전용
	void ForEachChildBool(UIComponent* c, const function<TraverseResult(UIComponent*)>& Func) noexcept; //무언가를 찾았으면 bool 반환으로 그만 하라는 함수
	bool ForEachChildWithRegion(UIComponent* c, const string& parentRegion, const function<bool(const string&, UIComponent*, bool)>& Func) noexcept; //소속된 region을 알려주면서 foreach
	void ForEachChildInSameRegion(UIComponent* c, const function<void(UIComponent*)>& Func) noexcept; //소속된 region만 도는 foreach

private:
	void ForEachRenderChildBFS(UIComponent* c, const function<TraverseResult(UIComponent*)>& Func) noexcept;
	void ForEachRenderChildDFS(UIComponent* c, const function<TraverseResult(UIComponent*)>& Func) noexcept;
};