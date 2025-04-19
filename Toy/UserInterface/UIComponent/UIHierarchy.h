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
	void ForEachChild(invocable<UIComponent*> auto&& Func) noexcept;
	bool ForEachChildUntilFail(predicate<UIComponent*> auto&& Func) noexcept; //���ϰ��� ��ȯ�ϴ� ���(�ε��Լ�����)
	bool ForEachChildPostUntilFail(predicate<UIComponent*> auto&& Func) noexcept; //���� ��ȸ�� ���ϰ� ��ȯ ���(binder�����)
	void ForEachChildBool(function<CResult(UIComponent*)> Func) noexcept; //���𰡸� ã������ bool ��ȯ���� �׸� �϶�� �Լ�
	//void ForEachChildConst(function<void(const UIComponent*)> Func) const noexcept; //�б�����
	void ForEachChildConst(invocable<const UIComponent*> auto&& Func) const noexcept;
	void ForEachChildBFS(StateFlag::Type flag, function<void(UIComponent*)> Func) noexcept; //BFS������ ����. �⺻��DFS

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