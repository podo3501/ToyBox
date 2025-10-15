#pragma once
#include "UIType.h"

class UIComponent;

//has로 리팩토링이 안되서 crtp로 리팩토링함.
template<typename T>
class UIHierarchy
{
public:
	~UIHierarchy() = default;
	UIHierarchy() = default;

	UIComponent* GetRoot() const noexcept;
	UIComponent* GetRegionRoot() const noexcept;
	UIComponent* GetParentRegionRoot() const noexcept;
	const string& GetMyRegion() const noexcept;

	//children을 순회하는 함수 모음
	void ForEachChildDFS(function<void(UIComponent*)> Func) noexcept;
	bool ForEachChildUntilFail(predicate<UIComponent*> auto&& Func) noexcept; //리턴값을 반환하는 재귀(로딩함수같은)
	bool ForEachChildPostUntilFail(predicate<UIComponent*> auto&& Func) noexcept; //후위 순회로 리턴값 반환 재귀(binder에사용)
	void ForEachChildBool(function<TraverseResult(UIComponent*)> Func) noexcept; //무언가를 찾았으면 bool 반환으로 그만 하라는 함수
	void ForEachChildConst(invocable<const UIComponent*> auto&& Func) const noexcept; //읽기전용
	bool ForEachChildWithRegion(const string& parentRegion, function<bool(const string&, UIComponent*, bool)> Func) noexcept; //소속된 region을 알려주면서 foreach
	void ForEachChildInSameRegion(function<void(UIComponent*)> Func) noexcept; //소속된 region만 도는 foreach
	void ForEachChildToRender(function<TraverseResult(UIComponent*)> Func) noexcept;

private:
	UIComponent* GetThis() const noexcept;
	void ForEachRenderChildBFS(function<TraverseResult(UIComponent*)> Func) noexcept;
	void ForEachRenderChildDFS(function<TraverseResult(UIComponent*)> Func) noexcept;

protected:
	//대부분은 직접적으로 m_children을 쓰고 UIModuleAsComponent의 children을 가지고 올때는 UIModule에서
	//children을 가져와야 하므로 virtual로 해서 다른 children을 가져오도록 하기 위해서 이 함수가 존재.
	virtual const vector<unique_ptr<UIComponent>>& GetChildren() const noexcept { return m_children; }

	UIComponent* m_parent{ nullptr };
	vector<unique_ptr<UIComponent>> m_children;
};

template<typename T>
void UIHierarchy<T>::ForEachChildDFS(function<void(UIComponent*)> Func) noexcept
{
	Func(GetThis());
	for (auto& child : m_children)
		if (child) child->ForEachChildDFS(Func);
}

template<typename T>
bool UIHierarchy<T>::ForEachChildUntilFail(predicate<UIComponent*> auto&& Func) noexcept
{
	if (!Func(GetThis())) return false;
	for (auto& child : m_children)
		if (child && !child->ForEachChildUntilFail(Func)) return false;
	return true;
}

template<typename T>
bool UIHierarchy<T>::ForEachChildPostUntilFail(predicate<UIComponent*> auto&& Func) noexcept
{
	for (auto& child : m_children)
		if (child && !child->ForEachChildPostUntilFail(Func)) return false;
	if (!Func(GetThis())) return false;
	return true;
}

template<typename T>
void UIHierarchy<T>::ForEachChildConst(invocable<const UIComponent*> auto&& Func) const noexcept
{
	Func(static_cast<const UIComponent*>(this));
	for (const auto& child : m_children)
		if (child) child->ForEachChildConst(Func);
}

