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

	UIComponent* GetRoot() noexcept;
	UIComponent* GetRegionRoot() noexcept;
	UIComponent* GetParentRegionRoot() noexcept;

	//children�� ��ȸ�ϴ� �Լ� ����
	void ForEachChild(invocable<UIComponent*> auto&& func) noexcept;
	bool ForEachChildUntilFail(predicate<UIComponent*> auto&& func) noexcept; //���ϰ��� ��ȯ�ϴ� ���(�ε��Լ�����)
	bool ForEachChildPostUntilFail(predicate<UIComponent*> auto&& func) noexcept; //���� ��ȸ�� ���ϰ� ��ȯ ���(binder�����)
	void ForEachChildBool(function<CResult(UIComponent*)> func) noexcept; //���𰡸� ã������ bool ��ȯ���� �׸� �϶�� �Լ�
	//void ForEachChildConst(function<void(const UIComponent*)> func) const noexcept; //�б�����
	void ForEachChildConst(invocable<const UIComponent*> auto&& func) const noexcept;
	void ForEachChildBFS(StateFlag::Type flag, function<void(UIComponent*)> func) noexcept; //BFS������ ����. �⺻��DFS

	//�̸� �� Region�� ���õ� �Լ���. �Լ��� �� ����� �ϴ� ���ǽ�Ŵ
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
void UIHierarchy<T>::ForEachChild(invocable<UIComponent*> auto&& func) noexcept
{
	func(GetThis());
	for (auto& child : m_children)
		if (child) child->ForEachChild(func);
}

template<typename T>
bool UIHierarchy<T>::ForEachChildUntilFail(predicate<UIComponent*> auto&& func) noexcept
{
	if (!func(GetThis())) return false;
	for (auto& child : m_children)
		if (child && !child->ForEachChildUntilFail(func)) return false;
	return true;
}

template<typename T>
bool UIHierarchy<T>::ForEachChildPostUntilFail(predicate<UIComponent*> auto&& func) noexcept
{
	for (auto& child : m_children)
		if (child && !child->ForEachChildPostUntilFail(func)) return false;
	if (!func(GetThis())) return false;
	return true;
}

template<typename T>
void UIHierarchy<T>::ForEachChildConst(invocable<const UIComponent*> auto&& func) const noexcept
{
	func(static_cast<const UIComponent*>(this));
	for (const auto& child : m_children)
		if (child) child->ForEachChildConst(func);
}