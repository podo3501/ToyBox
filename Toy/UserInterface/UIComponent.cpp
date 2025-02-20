#include "pch.h"
#include "UIComponent.h"
#include "../Utility.h"
#include "../InputManager.h"
#include "JsonOperation.h"

UIComponent::~UIComponent() = default;
UIComponent::UIComponent() :
	m_layout{ XMUINT2{}, Origin::LeftTop }
{}

UIComponent::UIComponent(const string& name, const UILayout& layout) noexcept :
	m_name{ name },
	m_layout{ layout }
{}

//��ӹ��� �������� ��������ڸ� ȣ���� �� �ִ�.
UIComponent::UIComponent(const UIComponent& other)
{
	m_name = other.m_name;
	m_layout = other.m_layout;
	m_region = other.m_region;
	m_stateFlag = other.m_stateFlag;
	m_transform = other.m_transform;

	ranges::transform(other.m_children, back_inserter(m_children), [this, &other](const auto& child) {
		auto component = child->CreateClone();
		component->SetParent(this);
		return move(component);
	});
}

bool UIComponent::EqualComponent(const UIComponent* lhs, const UIComponent* rhs) const noexcept
{
	if (lhs == nullptr && rhs == nullptr) return true;
	if (lhs == nullptr || rhs == nullptr) return false;
	if (lhs->m_name != rhs->m_name) return false;

	return true;
}

bool UIComponent::operator==(const UIComponent& o) const noexcept
{
	if (GetTypeID() != o.GetTypeID()) return false;

	ReturnIfFalse(tie(m_name, m_layout, m_region, m_stateFlag, m_transform) ==
		tie(o.m_name, o.m_layout, o.m_region, o.m_stateFlag, o.m_transform));
	ReturnIfFalse(EqualComponent(m_parent, o.m_parent));
	ReturnIfFalse(m_children.size() == o.m_children.size());
	ReturnIfFalse(ranges::equal(m_children, o.m_children, [](const auto& lhs, const auto& rhs) {
		return CompareUniquePtr(lhs, rhs); }));

	return true;
}

unique_ptr<UIComponent> UIComponent::Clone() const 
{ 
	return CreateClone();
}

bool UIComponent::LoadResources(ITextureLoad* load)
{
	return ForEachChildUntilFail([load](UIComponent* component) {
		return component->ImplementLoadResource(load);
		});
}

bool UIComponent::PostLoaded(ITextureController* texController)
{
	return ForEachChildUntilFail([texController](UIComponent* component) {
		return component->ImplementPostLoaded(texController);
		});
}

UITransform& UIComponent::GetTransform(UIComponent* component)
{
	return component->m_transform;
}

bool UIComponent::RecursiveUpdate(const XMINT2& position, bool active) noexcept
{
	if (!HasStateFlag(StateFlag::Update)) return true;

	ReturnIfFalse(ImplementUpdatePosition(position));
	if (active)
	{
		ReturnIfFalse(ImplementActiveUpdate());
		active = HasStateFlag(StateFlag::ActiveUpdate);
	}

	auto result = ranges::all_of(m_children, [this, &position, active](auto& child) {
		const auto& curPosition = GetTransform(child.get()).GetPosition(m_isDirty, m_layout, position);
		return child->RecursiveUpdate(curPosition, active);
		});

	return result;
}

bool UIComponent::ProcessUpdate() noexcept
{
	ReturnIfFalse(RecursiveUpdate({}, true));
	m_isDirty = false;
	return true;
}

void UIComponent::ProcessRenderTexture(ITextureRender* render)
{
	//StateFlag::Render�� �ؽ��Ŀ� �׸��� ������ ���� �ؽ��Ŀ� �׸��� ������. RenderTexture�� �����ϸ� ���� ȭ�鿡�� �ȳ��´�.
	//���� �ؽ��ķ� �ε��ϴ� ����� �ƴϰ� �ȴٸ� StateFlag::Render�� �������� �ǰ���.
	ForEachChildBFS(StateFlag::Render | StateFlag::RenderTexture, [render](UIComponent* component) {
		component->ImplementRender(render);
		});
}

void UIComponent::ProcessRender(ITextureRender* render)
{
	//9���� �̹����� ���� �����ε� 9���� �̹��� ���� �ٸ� �̹����� �÷��� ��� BFS�� �ƴϸ� �ؿ� �̹����� �ö�´�.
	//���� �ؿ� ������ ���� ���� �ö���µ� DFS(Depth First Search)�̸� ���� �ؿ� �ִ°� ���� ���߿� �׷����� �ʰ� �ȴ�.
	ForEachChildBFS(StateFlag::Render, [render](UIComponent* component) {
		component->ImplementRender(render);
		});
}

void UIComponent::SetStateFlag(StateFlag::Type flag, bool enabled) noexcept
{
	if (enabled)
	{
		m_stateFlag |= flag;

		//Render�� RenderTexture�� ���߿� �ϳ��� ���õǾ�� �Ѵ�.
		switch (flag)
		{
		case StateFlag::Render: SetStateFlag(StateFlag::RenderTexture, false); break;
		case StateFlag::RenderTexture: SetStateFlag(StateFlag::Render, false); break;
		}
	}
	else
		m_stateFlag &= ~flag;
}

//ũ�⸦ �ٲٸ� �� ������Ʈ�� �ڽĵ��� ��ġ���� �ٲ��ش�.
void UIComponent::ChangeSize(const XMUINT2& size) noexcept
{
	ranges::for_each(m_children, [this, &size](auto& child) {
		GetTransform(child.get()).AdjustPosition(size);
		});

	ApplySize(size);
}

bool UIComponent::ChangePosition(int index, const XMUINT2& size, const XMINT2& relativePos) noexcept
{
	if (index >= m_children.size()) return false;
	
	GetTransform(m_children[index].get()).SetRelativePosition(size, relativePos);
	MarkDirty();

	return true;
}

bool UIComponent::Rename(const string& name) noexcept
{
	if (m_name == name) return true;

	ReturnIfFalse(IsUniqueName(name, this));
	m_name = name;

	return true;
}

bool UIComponent::RenameRegion(const string& region) noexcept
{
	if (m_region == region) return true;

	ReturnIfFalse(IsUniqueRegion(region));
	m_region = region;

	return true;
}

void UIComponent::SerializeIO(JsonOperation& operation)
{
	operation.Process("Name", m_name);
	operation.Process("Layout", m_layout);
	operation.Process("Transform", m_transform);
	operation.Process("Region", m_region);
	operation.Process("StateFlag", m_stateFlag);
	operation.Process("Children", m_children);
	
	if (operation.IsWrite()) return;
	ranges::for_each(m_children, [this](auto& child) {
		child->SetParent(this);
		});
}

void UIComponent::MarkDirty() noexcept
{
	ForEachChild([](UIComponent* component) {
		component->m_isDirty = true;
		});
}

optional<XMINT2> UIComponent::GetRelativePosition() const noexcept
{
	return m_transform.GetRelativePosition();
}

bool UIComponent::SetRelativePosition(const XMINT2& relativePos) noexcept
{
	if (!m_parent) return false;
	m_transform.SetRelativePosition(m_parent->GetSize(), relativePos);
	m_parent->MarkDirty();
	return true;
}

Rectangle UIComponent::GetRectangle() const noexcept
{
	const XMINT2& curPosition = GetPosition();
	const XMUINT2& curSize = GetSize();
	return Rectangle(curPosition.x, curPosition.y, curSize.x, curSize.y);
}

const XMUINT2& UIComponent::GetSize() const noexcept
{
	return m_layout.GetSize();
}

XMINT2 UIComponent::GetPosition() const noexcept
{
	return GetPositionByLayout(m_transform.GetAbsolutePosition());
}

XMINT2 UIComponent::GetPositionByLayout(const XMINT2& position) const noexcept
{
	return position + m_layout.GetPosition();
}


UIComponent* UIComponent::GetChildComponent(size_t index) const noexcept
{
	if (index >= m_children.size()) return nullptr;
	return m_children[index].get();
}

vector<UIComponent*> UIComponent::GetChildComponents() const noexcept
{
	vector<UIComponent*> componentList;
	ranges::transform(m_children, back_inserter(componentList), [](const auto& child) {
		return child.get();
		});

	return componentList;
}

UIComponent* UIComponent::GetSiblingComponent(StateFlag::Type flag) const noexcept
{
	if (!m_parent) return {};
	auto components = m_parent->GetChildComponents();
	
	auto find = ranges::find_if(components, [flag](auto& component) {
		return component->HasStateFlag(flag);
		});
	if (find == components.end())
		return nullptr;

	return *find;
}


