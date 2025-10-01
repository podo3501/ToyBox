#include "pch.h"
#include "Utils/GeometryUtil.h"
#include "UIComponent.h"
#include "IRenderer.h"
#include "../TextureResourceBinder/TextureResourceBinder.h"
#include "Components/UIModuleAsComponent.h"
#include "Components/Panel.h"
#include "../UINameGenerator/UINameGenerator.h"
#include "../UIModule.h"
#include "GameConfig.h"

UIComponentEx::UIComponentEx(UIComponent* component) noexcept :
	m_component{ component },
	m_cachedUIModule{ nullptr }
{}

unique_ptr<UIComponent> UIComponentEx::AttachComponent(
	unique_ptr<UIComponent> child, const XMINT2& relativePos) noexcept
{
	ZoneScopedN("AttachComponent"); // ��ü �Լ� ����

	if (!m_component->HasStateFlag(StateFlag::Attach))
		return child;

	if (UINameGenerator* nameGen = GetNameGenerator())
	{
		bool success = child->ForEachChildWithRegion(m_component->GetMyRegion(),
			[nameGen](const string& region, UIComponent* component, bool isNewRegion) {
				auto [newRegion, newName] = nameGen->MakeNameOf(component->GetName(), region, component->GetTypeID(), isNewRegion);
				if (newName.empty())
					return false;

				if (isNewRegion) component->m_region = newRegion;
				component->m_name = newName;
				return true;
			});

		if (!success)
			return child;
	}

	child->SetParent(m_component);
	child->m_transform.SetRelativePosition(
		m_component->m_layout.GetSize(), relativePos);
	m_component->m_children.emplace_back(move(child));
	m_component->UpdatePositionsManually(true);

	return nullptr;
}

unique_ptr<UIComponent> UIComponentEx::AttachComponent(const string& region, const string& name,
	unique_ptr<UIComponent> child, const XMINT2& relativePos) noexcept
{
	UIComponent* find = FindComponent(region, name);
	if (!find) return child;

	return UIEx(find).AttachComponent(move(child), relativePos);
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

	if (UINameGenerator* nameGen = GetNameGenerator())	// �̸� ���� ó��
	{
		bool allRemoved = m_component->ForEachChildWithRegion(parent->GetMyRegion(),
			[nameGen](const string& region, UIComponent* component, bool isNewRegion) {
				if (nameGen->IsUnusedRegion(region)) //region�� ���� ���� detach �ϴٰ� �θ� ��忡�� region�� ���� ����̴�.
					return true;

				if (isNewRegion) //��尡 ���ο� region�̸� 
					return nameGen->RemoveRegion(region);
				return nameGen->RemoveName(region, component->GetName());
			});

		if (!allRemoved)
			return {};
	}

	unique_ptr<UIComponent> detach = DetachChild(parent, m_component);
	if (!detach) return {};

	return { move(detach), parent };
}

pair<unique_ptr<UIComponent>, UIComponent*> UIComponentEx::DetachComponent(const string& region, const string& name) noexcept
{
	UIComponent* find = FindComponent(region, name);
	if (!find) return {};

	return UIEx(find).DetachComponent();
}

bool UIComponentEx::Rename(const string& name) noexcept
{
	UINameGenerator* nameGen = GetNameGenerator();
	if (!nameGen)
	{
		m_component->m_name = name;
		return true;
	}

	const auto& region = m_component->GetMyRegion();
	if (!nameGen->IsUnusedName(region, name)) return false;

	nameGen->RemoveName(region, m_component->GetName());
	auto [_, newName] = nameGen->MakeNameOf(name, region, m_component->GetTypeID());
	m_component->m_name = move(newName);

	return true;
}

void UIComponentEx::AssignNamesInRegion(UIComponent* component, UINameGenerator* nameGen, const string& region) noexcept
{
	component->ForEachChildInSameRegion([nameGen, &region, component](UIComponent* curComponent) {
		auto result = nameGen->MakeNameOf(curComponent->GetName(), region, curComponent->GetTypeID());
		curComponent->m_name = move(result.second);
		if (curComponent == component)
			curComponent->m_region = move(result.first);
		});
}

bool UIComponentEx::RenameRegion(const string& region) noexcept
{
	UINameGenerator* nameGen = GetNameGenerator();
	if (!nameGen)	//�ӽ÷� ���� component�� ��� NameGenerator�� ����. ���߿� UIModule�� attach �ɶ� unique���� Ȯ���ϰ� �Ǵϱ� ������ �ٸ� ��忡 ���� region, �Ǵ� �̸��� �־ �������.
	{
		m_component->m_region = region;
		return true;
	}

	ReturnIfFalse(nameGen->IsUnusedRegion(region));

	UIComponent* rootRegionComponent = region.empty() ?
		m_component->GetParentRegionRoot() :
		m_component->GetRegionRoot();

	const string& rootRegion = rootRegionComponent->GetRegion();
	ReturnIfFalse(nameGen->RemoveRegion(rootRegion));
	if (rootRegionComponent != m_component)
	{
		const string& preRegion = m_component->GetRegion();
		if (!preRegion.empty())
		{
			ReturnIfFalse(nameGen->RemoveRegion(preRegion));
			m_component->m_region.clear();
		}

		if(!region.empty()) //region�� ���� ���ٸ� ���� region�� �̸��� ���Եǰ� �ȴ�.
			AssignNamesInRegion(m_component, nameGen, region);
	}
	AssignNamesInRegion(rootRegionComponent, nameGen, rootRegion);

	return true;
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
	UIComponent* foundComponent{ nullptr };

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

UIComponent* UIComponentEx::FindComponent(const string& region, const string& name) noexcept
{
	UIComponent* component = GetRegionComponent(region);
	if (component == nullptr) return nullptr;

	return UIEx(component).FindComponent(name);
}

vector<UIComponent*> UIComponentEx::GetRenderComponents(const XMINT2& pos) noexcept
{
	vector<UIComponent*> findList;
	m_component->ForEachChildToRender([&findList, &pos](UIComponent* comp) {
		if(Contains(comp->GetArea(), pos))
			findList.push_back(comp);
		});
	return findList;
}

XMUINT2 UIComponentEx::GetChildrenBoundsSize() const noexcept
{
	if (m_component == nullptr) return {};

	Rectangle totalArea{ m_component->GetArea() }; //�ʱⰪ�� �������� ������ 0, 0 ���� �����ϴ� ū �簢���� union�ȴ�.
	m_component->ForEachChildConst([&totalArea](const UIComponent* child) {
		const auto& area = child->GetArea();
		totalArea = Rectangle::Union(totalArea, area);
		});

	return GetSizeFromRectangle(totalArea);
}

bool UIComponentEx::IsPositionUpdated() const noexcept //?!? �̰� ���Ҷ� ���� �Լ�����? �׽�Ʈ ���̸� �׽�Ʈ������ ����.
{
	m_component->UpdatePositionsManually(true);
	auto clone = m_component->Clone();
	clone->UpdatePositionsManually(true); //Ŭ���� ���� ������Ʈ ��Ų�� ���� ������Ʈ�� �����ǰ� ��.
	return m_component->ForEachChildUntilFail([&clone](const UIComponent* child) {
		UIComponent* cloneComponent = UIEx(clone).FindComponent(child->GetMyRegion(), child->GetName());
		if (cloneComponent == nullptr) return false;
		return cloneComponent->GetPosition() == child->GetPosition();
		});
}

UIModule* UIComponentEx::GetUIModule(UIComponent* start) const
{
	UIComponent* current = start;

	while (current)
	{
		if (Panel* panel = ComponentCast<Panel*>(current))
		{
			if (UIModule* module = panel->GetUIModule())
				return module;
		}

		current = current->m_parent;
	}

	return nullptr;
}

UIModule* UIComponentEx::GetUIModule() const noexcept
{
	if (m_cachedUIModule) return m_cachedUIModule;
	
	m_cachedUIModule = GetUIModule(m_component);
	return m_cachedUIModule;
}

UINameGenerator* UIComponentEx::GetNameGenerator() const noexcept
{
	UIModule* uiModule = GetUIModule();
	return uiModule ? uiModule->GetNameGenerator() : nullptr;
}

void UIComponentEx::InvalidateUIModuleCache()
{
	m_cachedUIModule = nullptr;
	for (auto& child : m_component->m_children)
		UIEx(child).InvalidateUIModuleCache();
}

/////////////////////////////////////////////////////////////////

bool ChangeSizeX(UIComponent* c, uint32_t v) noexcept { return c->ChangeSize({ v, c->GetSize().y }); }
bool ChangeSizeX(UIComponent* c, const XMUINT2& s) noexcept { return ChangeSizeX(c, s.x); }
bool ChangeSizeY(UIComponent* c, uint32_t v) noexcept { return c->ChangeSize({ c->GetSize().x, v }); }
bool ChangeSizeY(UIComponent* c, const XMUINT2& s) noexcept { return ChangeSizeY(c, s.y); }