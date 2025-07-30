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

unique_ptr<UIComponent> UIComponentEx::AttachComponent(
	unique_ptr<UIComponent> child, const XMINT2& relativePos) noexcept
{
	ZoneScopedN("AttachComponent"); // 전체 함수 측정

	if (!m_component->HasStateFlag(StateFlag::Attach))
		return child;

	unique_ptr<UINameGenerator> newNameGen{ nullptr };
	UINameGenerator* nameGen = GetNameGenerator(newNameGen);

	{
		ZoneScopedN("GenerateUniqueName");

		UIComponent* regionComponent = m_component->GetRegionRoot();
		string parentRegion = regionComponent->GetRegion();

		bool result = child->ForEachChildWithRegion([nameGen, &parentRegion](const string& region, UIComponent* component) {
			bool isUniqueRegion{ false };
			string curRegion{ parentRegion };
			if (!region.empty() && parentRegion != region)
			{
				isUniqueRegion = true; //부모노드에 같은 region이 있으면 새 region(_1을 붙여서)으로 만들어야 한다.
				curRegion = region;
			}
				
			auto [makeRegion, makeName] = nameGen->MakeNameOf(component->GetName(), curRegion, component->GetTypeID(), isUniqueRegion);
			if (makeName.empty()) return false;

			if (makeRegion != parentRegion)
			{
				parentRegion = makeRegion;
				component->m_region = makeRegion;
			}

			component->m_name = makeName;
			return true;
			});
		if (!result) 
			return child;

		child->SetParent(m_component);
		child->m_transform.SetRelativePosition(
			m_component->m_layout.GetSize(), relativePos);
		m_component->m_children.emplace_back(move(child));
		m_component->UpdatePositionsManually(true);
	}

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

	unique_ptr<UINameGenerator> newNameGen;
	UINameGenerator* nameGen = GetNameGenerator(newNameGen); //?!? NameGenerator가 없을때는 리턴값을 nullptr로 처리해서 RemoveNameOf를 실행 시키지 않는게 맞는 방법 같다.

	UIComponent* parentRegionRoot = parent->GetRegionRoot();
	const string& parentRegion = parentRegionRoot->GetRegion();
	bool result = m_component->ForEachChildWithRegion([nameGen, &parentRegion](const string& region, UIComponent* component) {
		bool result{ false };
		if (parentRegion == region)
			result = nameGen->RemoveName(region, component->GetName());
		else
			result = nameGen->RemoveRegion(region); 
		//?!? 컨테이너가 _1, _2 이렇게 많이 나오는게 문제네. 왜 이렇게 들어가는지 확인해 보자. 아니지 _1, _2 이렇게 나와야한다. 이름도 _1, _2 이렇게 되나?
		if (!result)
			return result;
		return result;
		});
	if (!result) 
		return {};

	unique_ptr<UIComponent> detach = DetachChild(parent, m_component);
	if (detach == nullptr) return {};

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

	UIComponent* regionComponent = m_component->GetRegionRoot();
	const auto& region = regionComponent->GetRegion();

	if (!nameGen->IsUniqueName(region, name)) return false;

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
	if (!nameGen)	//임시로 만든 component인 경우 NameGenerator가 없다. 나중에 UIModule에 attach 될때 unique인지 확인하게 되니까 지금은 다른 노드에 같은 region, 또는 이름이 있어도 상관없다.
	{
		m_component->m_region = region;
		return true;
	}

	ReturnIfFalse(nameGen->IsUniqueRegion(region));

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

		if(!region.empty()) //region이 값이 없다면 상위 region에 이름이 포함되게 된다.
			AssignNamesInRegion(m_component, nameGen, region);
	}
	AssignNamesInRegion(rootRegionComponent, nameGen, rootRegion);

	return true;
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
	UIComponent* foundComponent{ nullptr };

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

	Rectangle totalArea{ m_component->GetArea() }; //초기값을 지정하지 않으면 0, 0 부터 시작하는 큰 사각형이 union된다.
	m_component->ForEachChildConst([&totalArea](const UIComponent* child) {
		const auto& area = child->GetArea();
		totalArea = Rectangle::Union(totalArea, area);
		});

	return GetSizeFromRectangle(totalArea);
}

bool UIComponentEx::IsPositionUpdated() const noexcept //?!? 이건 뭐할때 쓰는 함수이지? 테스트 용이면 테스트쪽으로 빼자.
{
	m_component->UpdatePositionsManually(true);
	auto clone = m_component->Clone();
	clone->UpdatePositionsManually(true); //클론을 만들어서 업데이트 시킨후 현재 컴포넌트와 포지션값 비교.
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

UINameGenerator* UIComponentEx::GetNameGenerator(unique_ptr<UINameGenerator>& nameGenerator) const noexcept
{
	UIModule* uiModule = GetUIModule();
	if (uiModule)
		return uiModule->GetNameGenerator();

	nameGenerator = make_unique<UINameGenerator>();
	return nameGenerator.get();
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