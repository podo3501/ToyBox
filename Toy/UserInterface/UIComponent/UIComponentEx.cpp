#include "pch.h"
#include "Shared/Utils/GeometryExt.h"
#include "UIComponent.h"
#include "IRenderer.h"
#include "../TextureResourceBinder/TextureResourceBinder.h"
#include "Components/UIModuleAsComponent.h"
#include "Components/Panel.h"
#include "../UINameGenerator/UINameGenerator.h"
#include "../UIModule.h"

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

	if (UINameGenerator* nameGen = GetNameGenerator())
	{
		bool success = child->ForEachChildWithRegion(m_component->GetMyRegion(),
			[nameGen](const string& region, UIComponent* component, bool isNewRegion) {
				auto namesOpt = nameGen->MakeNameOf(component->GetName(), region, component->GetTypeID(), isNewRegion);
				if (!namesOpt) return false;
				const auto& [newRegion, newName] = *namesOpt;
				
				if (isNewRegion) component->m_region = newRegion;
				component->m_name = newName;
				return true;
			});

		if (!success)
			return child;
	}

	child->SetParent(m_component);
	child->m_transform.ChangeRelativePosition(
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

	if (UINameGenerator* nameGen = GetNameGenerator())	// 이름 제거 처리
	{
		bool allRemoved = m_component->ForEachChildWithRegion(parent->GetMyRegion(),
			[nameGen](const string& region, UIComponent* component, bool isNewRegion) {
				if (nameGen->IsUnusedRegion(region)) //region이 없는 경우는 detach 하다가 부모 노드에서 region을 지운 경우이다.
					return true;

				if (isNewRegion) //노드가 새로운 region이면 
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
	auto namesOpt = nameGen->MakeNameOf(name, region, m_component->GetTypeID());
	if (!namesOpt) return false;
	m_component->m_name = namesOpt->second;

	return true;
}

void UIComponentEx::AssignNamesInRegion(UIComponent* component, UINameGenerator* nameGen, const string& region) noexcept
{
	component->ForEachChildInSameRegion([nameGen, &region, component](UIComponent* curComponent) {
		auto namesOpt = nameGen->MakeNameOf(curComponent->GetName(), region, curComponent->GetTypeID());
		curComponent->m_name = move(namesOpt->second);
		if (curComponent == component)
			curComponent->m_region = move(namesOpt->first);
		});
}

bool UIComponentEx::RenameRegion(const string& newRegion) noexcept
{
	UIComponent* c = m_component;
	UINameGenerator* nameGen = GetNameGenerator();

	//NameGenerator가 없으면 임시 컴포넌트. 단순히 지역 이름만 바꾼다.
	if (!nameGen)
	{
		c->m_region = newRegion;
		return true;
	}

	const string oldRegion = c->GetRegion();
	if (oldRegion == newRegion)
		return true;

	UIComponent* parentRoot = c->GetParentRegionRoot();
	auto Reassign = [&](UIComponent* target, const string& r) {
		AssignNamesInRegion(target, nameGen, r);
		};

	if (newRegion == "") // region이 "" 라면 기존 region을 삭제 처리한다.
		return RemoveAndMergeRegion(c, parentRoot, nameGen, oldRegion);

	//새 region 이름이 이미 존재하면 중복 처리
	if (!nameGen->IsUnusedRegion(newRegion))
		return false;

	return ReplaceAndMergeRegion(c, parentRoot, nameGen, oldRegion, newRegion);
}

bool UIComponentEx::RemoveAndMergeRegion(UIComponent* c, UIComponent* parentRoot, 
	UINameGenerator* nameGen, const string& oldRegion) noexcept
{
	auto Reassign = [&](UIComponent* target, const string& r) {
		AssignNamesInRegion(target, nameGen, r);
		};

	// 기존 region 제거
	ReturnIfFalse(nameGen->RemoveRegion(oldRegion));
	c->m_region.clear();

	// root 여부에 따라 재정렬 대상 결정
	UIComponent* target = (c == parentRoot) ? c : parentRoot;
	const string& targetRegion = target->GetRegion();

	ReturnIfFalse(nameGen->RemoveRegion(targetRegion));
	Reassign(target, targetRegion);
	return true;
}

bool UIComponentEx::ReplaceAndMergeRegion(UIComponent* c, UIComponent* parentRoot,
	UINameGenerator* nameGen, const string& oldRegion, const string& newRegion) noexcept
{
	auto Reassign = [&](UIComponent* target, const string& r) {
		AssignNamesInRegion(target, nameGen, r);
		};

	//기존 region을 제거하고 새 region 재할당
	ReturnIfFalse(nameGen->RemoveRegion(oldRegion));
	Reassign(c, newRegion);

	//이전에 region이 없었던 경우 상위도 갱신
	if (oldRegion == "" && c != parentRoot)
	{
		string parentRegion = parentRoot->GetRegion();
		ReturnIfFalse(nameGen->RemoveRegion(parentRegion));
		Reassign(parentRoot, parentRegion);
	}

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
		if (!curRegion.empty() && region != curRegion) return TraverseResult::Stop; //Region 루트가 아닌 새로운 region이 나왔을때 

		if (component->GetName() == name)
		{
			foundComponent = component;
			return TraverseResult::Stop;
		}

		return TraverseResult::Continue;
		});

	return foundComponent;
}

//UIComponent* UIComponentEx::FindRegionComponent(const string& findRegion) noexcept
//{
//	UIComponent* root = m_component->GetRegionRoot();
//	if (!root) root = m_component->GetRoot();
//	const string& rootRegion = root->GetRegion();
//	UIComponent* foundComponent{ nullptr };
//
//	root->ForEachChildBool([&foundComponent, &rootRegion, &findRegion](UIComponent* component) {
//		const string& curRegion = component->GetRegion();
//		if (curRegion.empty()) return TraverseResult::Continue;
//
//		if (curRegion == findRegion)
//		{
//			foundComponent = component;
//			return TraverseResult::Stop;
//		}
//
//		if (rootRegion != curRegion) return TraverseResult::Stop; //Region 루트가 아닌 새로운 region이 나왔을때
//
//		return TraverseResult::Continue;
//		});
//
//	return foundComponent;
//}

UIComponent* UIComponentEx::FindRegionComponent(const string& findRegion) noexcept
{
	UIComponent* root = m_component->GetRoot();
	UIComponent* foundComponent = nullptr;

	root->ForEachChildBool([&](UIComponent* c) {
		if (c->GetRegion() == findRegion)
		{
			foundComponent = c;
			return TraverseResult::Stop;
		}
		return TraverseResult::Continue;
		});

	return foundComponent;
}

UIComponent* UIComponentEx::FindComponent(const string& region, const string& name) noexcept
{
	UIComponent* component = FindRegionComponent(region);
	if (component == nullptr) return nullptr;

	return UIEx(component).FindComponent(name);
}

vector<UIComponent*> UIComponentEx::PickComponents(const XMINT2& pos) noexcept
{
	vector<UIComponent*> findList;
	m_component->ForEachChildToRender([&findList, &pos](UIComponent* comp) {
		const bool inside = Contains(comp->GetArea(), pos);

		if (comp->GetTypeID() == ComponentID::RenderTexture && !inside) 
			return TraverseResult::ChildrenSkip; // RenderTexture는 영역 밖이면 자식 탐색 중단

		if (inside) // 영역 안이면 리스트에 추가
			findList.push_back(comp);

		return TraverseResult::Continue;
		});
	ranges::reverse(findList); //앞으로 넣어주는 것보다 push_back 하고 reverse 하는게 더 빠르다. vector가 단순 배열이라 캐쉬가 좋기 때문에 이걸로 한다.	 
	return findList;
}

vector<IMouseEventReceiver*> UIComponentEx::PickMouseReceivers(const XMINT2& pos) noexcept
{
	vector<IMouseEventReceiver*> findList;
	m_component->ForEachChildToRender([&findList, &pos](UIComponent* comp) {
		if (comp->GetTypeID() == ComponentID::RenderTexture && !Contains(comp->GetArea(), pos))
			return TraverseResult::ChildrenSkip; // RenderTexture 영역 밖이면 자식 탐색 중단

		if (auto eventReceiver = comp->AsMouseEventReceiver(); eventReceiver) { // 마우스 이벤트 수신 가능한지 먼저 확인
			if (Contains(comp->GetArea(), pos))
				findList.push_back(eventReceiver);
		}

		return TraverseResult::Continue;
		});
	ranges::reverse(findList); //앞으로 넣어주는 것보다 push_back 하고 reverse 하는게 더 빠르다. vector가 단순 배열이라 캐쉬가 좋기 때문에 이걸로 한다.	 
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