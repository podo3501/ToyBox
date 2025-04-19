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

	UIComponent* GetRoot() noexcept;
	UIComponent* GetRegionRoot() noexcept;
	UIComponent* GetParentRegionRoot() noexcept;

	//children을 순회하는 함수 모음
	void ForEachChild(invocable<UIComponent*> auto&& Func) noexcept;
	bool ForEachChildUntilFail(predicate<UIComponent*> auto&& Func) noexcept; //리턴값을 반환하는 재귀(로딩함수같은)
	bool ForEachChildPostUntilFail(predicate<UIComponent*> auto&& Func) noexcept; //후위 순회로 리턴값 반환 재귀(binder에사용)
	void ForEachChildBool(function<CResult(UIComponent*)> Func) noexcept; //무언가를 찾았으면 bool 반환으로 그만 하라는 함수
	//void ForEachChildConst(function<void(const UIComponent*)> Func) const noexcept; //읽기전용
	void ForEachChildConst(invocable<const UIComponent*> auto&& Func) const noexcept;
	void ForEachChildBFS(StateFlag::Type flag, function<void(UIComponent*)> Func) noexcept; //BFS용으로 만듦. 기본은DFS

	//이름 및 Region에 관련된 함수들. 함수가 길어서 여기로 일단 대피시킴
	bool IsUniqueRegion(const string& name) noexcept;
	bool IsUniqueName(const string& name, UIComponent* self) noexcept;
	bool IsUniqueRegionName(const string& name, UIComponent* self) noexcept;
	string CreateNewName(UIComponent* attaching) noexcept;
	string CreateNewRegionName(UIComponent* attaching) noexcept;
	void GenerateUniqueName(UIComponent* attachingBlock) noexcept;
	void GenerateUniqueRegionName(UIComponent* attachingBlock) noexcept;

private:
	UIComponent* GetThis() noexcept;

protected:
	UIComponent* m_parent{ nullptr };
	vector<unique_ptr<T>> m_children;
};

template<typename T>
void UIHierarchy<T>::ForEachChild(invocable<UIComponent*> auto&& Func) noexcept
{
	Func(GetThis());
	for (auto& child : m_children)
		if (child) child->ForEachChild(Func);
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