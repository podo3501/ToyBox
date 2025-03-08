#include "pch.h"
#include "../Utility.h"
#include "UIComponent.h"

UIComponentEx::UIComponentEx(UIComponent* component) noexcept :
	m_component{ component }
{}

unique_ptr<UIComponent> UIComponentEx::AttachComponent(
	unique_ptr<UIComponent> child, const XMINT2& relativePos) noexcept
{
	if (!m_component->HasStateFlag(StateFlag::Attach)) return child;

	m_component->GenerateUniqueName(child.get());
	m_component->GenerateUniqueRegionName(child.get());
	child->SetParent(m_component);
	child->m_transform.SetRelativePosition(m_component->m_layout.GetSize(), relativePos); //�θ� ������� ���� ��ġ�� ���ؾ� ������� ��ġ���� ���� �� �ִ�.
	auto childPtr = child.get();
	m_component->m_children.emplace_back(move(child));
	childPtr->OnAttached(m_component);

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

	detached->UnlinkAndRefresh();

	return detached;
}

pair<unique_ptr<UIComponent>, UIComponent*> UIComponentEx::DetachComponent() noexcept
{
	UIComponent* parent = m_component->m_parent;
	if (!parent || !parent->HasStateFlag(StateFlag::Detach)) return {};

	return { move(DetachChild(parent, m_component)), parent };
}

UIComponent* UIComponentEx::FindComponent(const string& name) noexcept
{
	UIComponent* root = m_component->GetRegionRoot();
	if (!root) root = m_component->GetRoot(); //Region�� ���� ��� root�� region root�� �ǰ� ""�� region name�̴�.
	const string& region = root->GetRegion();
	UIComponent* foundComponent = nullptr;

	root->ForEachChildBool([this, &foundComponent, &name, &region](UIComponent* component) {
		const string& curRegion = component->GetRegion();
		if (!curRegion.empty() && region != curRegion) return CResult::SkipChildren; //Region ��Ʈ�� �ƴ� ���ο� region�� �������� 

		if (component->GetName() == name)
		{
			foundComponent = component;
			return CResult::SkipChildren;
		}

		return CResult::Success;
		});

	return foundComponent;
}

UIComponent* UIComponentEx::GetRegionComponent(const string& findRegion) noexcept
{
	UIComponent* root = m_component->GetRegionRoot();
	if (!root) root = m_component->GetRoot();
	const string& rootRegion = root->GetRegion();
	UIComponent* foundComponent = nullptr;

	root->ForEachChildBool([&foundComponent, &rootRegion, &findRegion](UIComponent* component) {
		const string& curRegion = component->GetRegion();
		if (curRegion.empty()) return CResult::Success;

		if (curRegion == findRegion)
		{
			foundComponent = component;
			return CResult::SkipChildren;
		}

		if (rootRegion != curRegion) return CResult::SkipChildren; //Region ��Ʈ�� �ƴ� ���ο� region�� ��������

		return CResult::Success;
		});

	return foundComponent;
}

vector<UIComponent*> UIComponentEx::GetComponents(const XMINT2& pos) noexcept
{
	vector<UIComponent*> findList;
	m_component->ForEachChildBFS(StateFlag::Render | StateFlag::RenderTexture, [&findList, &pos](UIComponent* comp) {
		if(Contains(comp->GetArea(), pos))
			findList.push_back(comp);
		});
	return findList;
}

Rectangle UIComponentEx::GetTotalChildSize(const UIComponent* component) noexcept
{
	if (component == nullptr) return {};

	Rectangle totalArea{ component->GetArea() }; //�ʱⰪ�� �������� ������ 0, 0 ���� �����ϴ� ū �簢���� union�ȴ�.
	component->ForEachChildConst([&totalArea](const UIComponent* child) {
		const auto& area = child->GetArea();
		totalArea = Rectangle::Union(totalArea, area);
		});

	return totalArea;
}

XMUINT2 UIComponentEx::GetTotalChildSize() noexcept
{
	const Rectangle& totalRectangle = GetTotalChildSize(m_component);
	return {
		static_cast<uint32_t>(totalRectangle.width),
		static_cast<uint32_t>(totalRectangle.height) };
}

/////////////////////////////////////////////////////////////////

bool ChangeSizeX(UIComponent* c, uint32_t v) noexcept { return c->ChangeSize({ v, c->GetSize().y }); }
bool ChangeSizeY(UIComponent* c, uint32_t v) noexcept { return c->ChangeSize({ c->GetSize().x, v }); }