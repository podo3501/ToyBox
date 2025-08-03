#pragma once
#include "UIType.h"

class UIComponent;

//has�� �����丵�� �ȵǼ� crtp�� �����丵��.
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

	//children�� ��ȸ�ϴ� �Լ� ����
	void ForEachChildDFS(function<void(UIComponent*)> Func) noexcept;
	bool ForEachChildUntilFail(predicate<UIComponent*> auto&& Func) noexcept; //���ϰ��� ��ȯ�ϴ� ���(�ε��Լ�����)
	bool ForEachChildPostUntilFail(predicate<UIComponent*> auto&& Func) noexcept; //���� ��ȸ�� ���ϰ� ��ȯ ���(binder�����)
	void ForEachChildBool(function<CResult(UIComponent*)> Func) noexcept; //���𰡸� ã������ bool ��ȯ���� �׸� �϶�� �Լ�
	void ForEachChildConst(invocable<const UIComponent*> auto&& Func) const noexcept; //�б�����
	bool ForEachChildWithRegion(const string& parentRegion, function<bool(const string&, UIComponent*, bool)> Func) noexcept; //�Ҽӵ� region�� �˷��ָ鼭 foreach
	bool ForEachChildWithRegion(function<bool(const string&, UIComponent*)> Func) noexcept; 
	void ForEachChildInSameRegion(function<void(UIComponent*)> Func) noexcept; //�Ҽӵ� region�� ���� foreach
	void ForEachChildToRender(function<void(UIComponent*)> Func) noexcept;

private:
	UIComponent* GetThis() const noexcept;
	void ForEachRenderChildBFS(function<void(UIComponent*)> Func) noexcept;
	void ForEachRenderChildDFS(function<void(UIComponent*)> Func) noexcept;

protected:
	UIComponent* m_parent{ nullptr };
	vector<unique_ptr<T>> m_children;
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

