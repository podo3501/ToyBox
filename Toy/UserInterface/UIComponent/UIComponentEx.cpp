#include "pch.h"
#include "Utility.h"
#include "UIComponent.h"
#include "../Include/IRenderer.h"
#include "../TextureResourceBinder/TextureResourceBinder.h"
#include "Components/UIModuleAsComponent.h"
#include "Components/Panel.h"
#include "../UINameGenerator.h"
#include "../UIModule.h"
#include "Config.h"

UIComponentEx::UIComponentEx(UIComponent* component) noexcept :
	m_component{ component },
	m_cachedUIModule{ nullptr }
{}
//
//unique_ptr<UIComponent> UIComponentEx::AttachComponent(
//	unique_ptr<UIComponent> child, const XMINT2& relativePos) noexcept
//{
//	if (!m_component->HasStateFlag(StateFlag::Attach)) return child;
//
//	m_component->GenerateUniqueName(child.get());
//	m_component->GenerateUniqueRegionName(child.get());
//	child->SetParent(m_component);
//	child->m_transform.SetRelativePosition(m_component->m_layout.GetSize(), relativePos); //부모 사이즈와 나의 위치를 비교해야 상대적인 위치값을 구할 수 있다.
//
//	m_component->m_children.emplace_back(move(child));
//	m_component->UpdatePositionsManually(true);
//
//	return nullptr;
//}

//unique_ptr<UIComponent> UIComponentEx::AttachComponent(
//	unique_ptr<UIComponent> child, const XMINT2& relativePos) noexcept
//{
//	ZoneScopedN("AttachComponent"); // 전체 함수 측정
//
//	if (!m_component->HasStateFlag(StateFlag::Attach))
//		return child;
//
//	{
//		ZoneScopedN("GenerateUniqueNames");
//		m_component->GenerateUniqueName(child.get());
//	}
//
//	{
//		ZoneScopedN("GenerateUniqueRegionName");
//		m_component->GenerateUniqueRegionName(child.get());
//	}
//
//	{
//		ZoneScopedN("SetParent");
//		child->SetParent(m_component);
//	}
//
//	{
//		ZoneScopedN("SetRelativePosition");
//		child->m_transform.SetRelativePosition(
//			m_component->m_layout.GetSize(), relativePos);
//	}
//
//	{
//		ZoneScopedN("PushBackChild");
//		m_component->m_children.emplace_back(move(child));
//	}
//
//	{
//		ZoneScopedN("UpdateLayout");
//		m_component->UpdatePositionsManually(true);
//	}
//
//	return nullptr;
//}

unique_ptr<UIComponent> UIComponentEx::AttachComponent(
	unique_ptr<UIComponent> child, const XMINT2& relativePos) noexcept
{
	if (!m_component->HasStateFlag(StateFlag::Attach))
		return child;

	m_component->GenerateUniqueRegionName(child.get());

	UIComponent* regionComponent = m_component->GetParentRegionRoot();
	const auto& region = regionComponent->GetRegion();

	unique_ptr<UINameGenerator> newNameGen{ nullptr };
	UINameGenerator* nameGen{ nullptr };
	UIModule* uiModule = GetUIModule();
	if (uiModule)
		nameGen = uiModule->GetNameGenerator();
	else
	{
		newNameGen = make_unique<UINameGenerator>();
		nameGen = newNameGen.get();
	}

	child->ForEachChildUntilFail([this, nameGen, &region](UIComponent* component) {
		auto makeName = nameGen->MakeNameOf(component->GetName(), region, component->GetTypeID());
		if (makeName.empty()) return false;

		component->m_name = makeName;
		return true;
		});

	child->SetParent(m_component);
	child->m_transform.SetRelativePosition(
		m_component->m_layout.GetSize(), relativePos);
	m_component->m_children.emplace_back(move(child));
	m_component->UpdatePositionsManually(true);

	return nullptr;
}

unique_ptr<UIComponent> UIComponentEx::AttachComponent(UINameGenerator* generator,
	unique_ptr<UIComponent> child, const XMINT2& relativePos) noexcept
{
	if (!m_component->HasStateFlag(StateFlag::Attach))
		return child;

	UIComponent* regionComponent = m_component->GetParentRegionRoot();
	const auto& region = regionComponent->GetRegion();

	child->ForEachChildUntilFail([this, generator, &region](UIComponent* component) {
		auto makeName = generator->MakeNameOf(component->GetName(), region, component->GetTypeID());
		if (makeName.empty()) return false;

		component->m_name = makeName;
		return true;
		});
	
	return AttachComponent(move(child), relativePos);
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

pair<unique_ptr<UIComponent>, UIComponent*> UIComponentEx::DetachComponent(UINameGenerator* generator) noexcept
{
	UIComponent* regionComponent = m_component->GetParentRegionRoot();
	const auto& region = regionComponent->GetRegion();

	m_component->ForEachChildUntilFail([this, generator, &region](UIComponent* component) {
		auto& uniqueName = component->GetUniqueName();
		if (uniqueName.empty()) return false;

		if (generator->RemoveNameOf(region, uniqueName)) //?!? 인자를 Component로 보내서 지우고 이름을 바꾸면 좋지 않을까? attach도 그렇게 하는게.
			component->SetUniqueName("");

		return true;
		});

	return DetachComponent();
}

void UIComponentEx::Rename(UINameGenerator* generator, const string& name) noexcept
{
	UIComponent* regionComponent = m_component->GetParentRegionRoot();
	const auto& region = regionComponent->GetRegion();

	generator->RemoveNameOf(region, m_component->GetUniqueName());
	m_component->SetUniqueName(name);
}

UIComponent* UIComponentEx::FindComponent(const string& name) noexcept
{
	UIComponent* root = m_component->GetRegionRoot();
	if (!root) root = m_component->GetRoot(); //Region이 없을 경우 root가 region root가 되고 ""가 region name이다.
	const string& region = root->GetRegion();
	UIComponent* foundComponent = nullptr;

	root->ForEachChildBool([this, &foundComponent, &name, &region](UIComponent* component) {
		const string& curRegion = component->GetRegion();
		if (!curRegion.empty() && region != curRegion) return CResult::SkipChildren; //Region 루트가 아닌 새로운 region이 나왔을때 

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

		if (rootRegion != curRegion) return CResult::SkipChildren; //Region 루트가 아닌 새로운 region이 나왔을때

		return CResult::Success;
		});

	return foundComponent;
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

	Rectangle totalArea{ m_component->GetArea() }; //초기값을 지정하지 않으면 0, 0 부터 시작하는 큰 사각형이 union된다.
	m_component->ForEachChildConst([&totalArea](const UIComponent* child) {
		const auto& area = child->GetArea();
		totalArea = Rectangle::Union(totalArea, area);
		});

	return GetSizeFromRectangle(totalArea);
}

bool UIComponentEx::IsPositionUpdated() const noexcept
{
	m_component->UpdatePositionsManually(true);
	auto clone = m_component->Clone();
	clone->UpdatePositionsManually(true); //클론을 만들어서 업데이트 시킨후 현재 컴포넌트와 포지션값 비교.
	return m_component->ForEachChildUntilFail([&clone](const UIComponent* child) {
		UIComponent* cloneComponent = UIEx(clone).FindComponent(child->GetName());
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