#include "pch.h"
#include "../Utility.h"
#include "UIComponent.h"

UIComponentEx::UIComponentEx(UIComponent* component) noexcept :
	m_component{ component }
{}

unique_ptr<UIComponent> UIComponentEx::AttachComponent(
	unique_ptr<UIComponent> child, const XMINT2& relativePos) noexcept
{
	if (!m_component->IsAttachable()) return child;

	m_component->GenerateUniqueName(child.get());
	child->SetParent(m_component);
	child->m_transform.SetRelativePosition(m_component->m_layout.GetSize(), relativePos); //�θ� ������� ���� ��ġ�� ���ؾ� ������� ��ġ���� ���� �� �ִ�.
	m_component->m_children.emplace_back(move(child));
	m_component->MarkDirty();

	return nullptr;
}

unique_ptr<UIComponent> UIComponentEx::DetachChild(UIComponent* parent, UIComponent* detach) noexcept
{
	auto find = ranges::find_if(parent->m_children, [detach](auto& child) {
		return (child.get() == detach);
		});
	if (find == parent->m_children.end()) return nullptr;

	unique_ptr<UIComponent> detached = move(*find);
	parent->m_children.erase(find);

	detached->m_parent = nullptr;
	detached->m_transform.Clear();
	detached->MarkDirty();
	detached->ProcessUpdate({});

	return detached;
}

pair<unique_ptr<UIComponent>, UIComponent*> UIComponentEx::DetachComponent() noexcept
{
	if (!m_component->IsDetachable()) return {};
	if (!m_component->m_parent) return {};

	UIComponent* parent = m_component->m_parent; //DetachComponent�� �ϸ� parent�� nullptr�� ���õȴ�.
	return { move(DetachChild(parent, m_component)), parent };
}

UIComponent* UIComponentEx::GetComponent(const string& name) noexcept
{
	UIComponent* root = m_component->GetRoot();
	UIComponent* foundComponent = nullptr;

	root->ForEachChild([&foundComponent, &name](UIComponent* child) {
		if (child->GetName() == name)
			foundComponent = child;
		});

	return foundComponent;
}

vector<UIComponent*> UIComponentEx::GetComponents(const XMINT2& pos) noexcept
{
	vector<UIComponent*> findList;
	m_component->ForEachChildBFS([&findList, &pos](UIComponent* comp) {
		const auto& curPosition = pos - comp->m_transform.GetAbsolutePosition();
		if (comp->IsArea(curPosition))
			findList.push_back(comp);
		});
	return findList;
}

Rectangle UIComponentEx::GetTotalChildSize(const UIComponent* component) noexcept
{
	if (component == nullptr) return {};

	Rectangle rect{ component->GetRectangle() }; //�ʱⰪ�� �������� ������ 0, 0 ���� �����ϴ� ū �簢���� union�ȴ�.
	component->ForEachChildConst([&rect](const UIComponent* child) {
		const auto& curRect = child->GetRectangle();
		rect = Rectangle::Union(rect, curRect);
		});

	return rect;
}

XMUINT2 UIComponentEx::GetTotalChildSize() noexcept
{
	const Rectangle& totalRectangle = GetTotalChildSize(m_component);
	return {
		static_cast<uint32_t>(totalRectangle.width),
		static_cast<uint32_t>(totalRectangle.height) };
}