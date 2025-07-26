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
	ZoneScopedN("AttachComponent"); // ��ü �Լ� ����

	if (!m_component->HasStateFlag(StateFlag::Attach))
		return child;

	unique_ptr<UINameGenerator> newNameGen{ nullptr };
	UINameGenerator* nameGen = GetNameGenerator(newNameGen);

	//{
	//	ZoneScopedN("GenerateUniqueRegionName");
	//	child->m_region = nameGen->MakeRegionOf(child->GetRegion());
	//}

	{
		ZoneScopedN("GenerateUniqueName");

		//UIComponent* regionComponent = child->GetRegionRoot();
		//string region{ regionComponent->GetRegion() };
		//if (region.empty())
		//{
		//	regionComponent = m_component->GetRegionRoot();
		//	region = regionComponent->GetRegion();
		//}

		//child->ForEachChildUntilFail([this, nameGen, &region](UIComponent* component) { //?!? ForEachChildWithRegion�Լ��� ��� �ҵ�. 
		//	auto makeName = nameGen->MakeNameOf(component->GetName(), region, component->GetTypeID());
		//	if (makeName.empty()) return false;

		//	component->m_name = makeName;
		//	return true;
		//	});

		UIComponent* regionComponent = m_component->GetRegionRoot();
		string parentRegion = regionComponent->GetRegion();

		child->ForEachChildWithRegion([nameGen, &parentRegion](const string& region, UIComponent* component) {
			//bool isUniqueRegion = !region.empty(); //child�� ���� ������ �ִ� region�� attach�ɶ����� ����ũ�ϰ� �̸��� ������ ��� �Ѵ�.
			//string curRegion = isUniqueRegion ? region : parentRegion;

			bool isUniqueRegion{ false };
			string curRegion{ parentRegion };
			if (!region.empty() && parentRegion != region)
			{
				isUniqueRegion = true;
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
	UINameGenerator* nameGen = GetNameGenerator(newNameGen); //?!? NameGenerator�� �������� ���ϰ��� nullptr�� ó���ؼ� RemoveNameOf�� ���� ��Ű�� �ʴ°� �´� ��� ����.

	UIComponent* parentRegionRoot = parent->GetRegionRoot();
	const string& parentRegion = parentRegionRoot->GetRegion();
	m_component->ForEachChildWithRegion([nameGen, &parentRegion](const string& region, UIComponent* component) {
		if (parentRegion == region)
			nameGen->RemoveName(region, component->GetName());
		else
			nameGen->RemoveRegion(region);
		});

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
	if (!nameGen)	//�ӽ÷� ���� component�� ��� NameGenerator�� ����. ���߿� UIModule�� attach �ɶ� unique���� Ȯ���ϰ� �Ǵϱ� ������ �ٸ� ��忡 ���� region, �Ǵ� �̸��� �־ �������.
	{
		m_component->m_region = region;
		return true;
	}

	ReturnIfFalse(nameGen->IsUniqueRegion(region));

	if (m_component->GetRegion().empty()) //����ִٸ� ��� �����ִ� region�̱� ������ ���ο� region�� ������ �Ѵ�.
	{
		UIComponent* regionComponent = m_component->GetRegionRoot();
		string curRegion = regionComponent->GetRegion();

		ReturnIfFalse(nameGen->RemoveRegion(curRegion));	//���� region�� �Ҽӵ� name�� ���⼭ �� �������. �׷��� �Ҽӵ� component�� ���鼭 �̸��� �ٽ� �־��־�� �Ѵ�.
		m_component->m_region = nameGen->MakeRegionOf(region);
		string newRegion = m_component->GetRegion();

		if (regionComponent != m_component)	//region�� ���� ��������� region�� �ΰ��� �ɶ� ���� region�� ���θ����.
			AssignNamesInRegion(regionComponent, nameGen, curRegion);
		AssignNamesInRegion(m_component, nameGen, newRegion);
	}
	else
	{
		ReturnIfFalse(nameGen->RenameRegion(m_component->m_region, region));
		m_component->m_region = region;
	}

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