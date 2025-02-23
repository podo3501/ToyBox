#include "pch.h"
#include "UIComponent.h"
#include "../Utility.h"
#include "../InputManager.h"
#include "JsonOperation.h"
#include "StepTimer.h"

UIComponent::~UIComponent() = default;
UIComponent::UIComponent() :
	m_layout{ XMUINT2{}, Origin::LeftTop }
{}

UIComponent::UIComponent(const string& name, const UILayout& layout) noexcept :
	m_name{ name },
	m_layout{ layout }
{}

//상속받은 곳에서만 복사생성자를 호출할 수 있다.
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

void UIComponent::UnlinkAndRefresh() noexcept
{
	m_parent = nullptr;
	m_transform.Clear();
	MarkDirty();
	UpdatePositionsManually();
}

unique_ptr<UIComponent> UIComponent::Clone() const 
{ 
	auto clone = CreateClone();
	clone->UnlinkAndRefresh(); //Clone의 root는 위치값을 초기화 시켜준다.
	return clone;
}

bool UIComponent::LoadResources(ITextureLoad* load)
{
	return ForEachChildUntilFail([load](UIComponent* component) {
		return component->ImplementLoadResource(load);
		});
}

bool UIComponent::PostLoaded(ITextureController* texController)
{
	ReturnIfFalse(UpdatePositionsManually());
	return ForEachChildUntilFail([texController](UIComponent* component) {
		return component->ImplementPostLoaded(texController);
		});
}

UITransform& UIComponent::GetTransform(UIComponent* component)
{
	return component->m_transform;
}

const XMINT2& UIComponent::GetUpdatedPosition(UIComponent* component, const XMINT2& parentPos) noexcept
{
	return GetTransform(component).GetUpdatedPosition(m_isDirty, m_layout, parentPos);
}

bool UIComponent::RecursiveUpdatePositionsManually(const DX::StepTimer& timer, const XMINT2& position) noexcept
{
	ReturnIfFalse(ImplementUpdatePosition(timer, position));

	bool result = ranges::all_of(m_children, [this, &timer, &position](auto& child) {
		return child->RecursiveUpdatePositionsManually(timer, GetUpdatedPosition(child.get(), position));
		});
	m_isDirty = false;

	return result;
}

bool UIComponent::UpdatePositionsManually(const XMINT2& position) noexcept
{
	DX::StepTimer dummyTimer; //임시타이머이다. 이 타이머는 사용하지 않는다.
	return RecursiveUpdatePositionsManually(dummyTimer, position);
}

bool UIComponent::RecursiveUpdate(const DX::StepTimer& timer, const XMINT2& position, bool active) noexcept
{
	if (!HasStateFlag(StateFlag::Update)) return true;

	ReturnIfFalse(ImplementUpdatePosition(timer, position));
	if (active)
	{
		ReturnIfFalse(ImplementActiveUpdate());
		active = HasStateFlag(StateFlag::ActiveUpdate);
	}

	bool result = ranges::all_of(m_children, [this, &timer, &position, active](auto& child) {
		return child->RecursiveUpdate(timer, GetUpdatedPosition(child.get(), position), active);
		});
	m_isDirty = false;

	return result;
}

bool UIComponent::ProcessUpdate(const DX::StepTimer& timer) noexcept
{
	return RecursiveUpdate(timer);
}

void UIComponent::ProcessRender(ITextureRender* render)
{
	//9방향 이미지는 같은 레벨인데 9방향 이미지 위에 다른 이미지를 올렸을 경우 BFS가 아니면 밑에 이미지가 올라온다.
	//가장 밑에 레벨이 가장 위에 올라오는데 DFS(Depth First Search)이면 가장 밑에 있는게 가장 나중에 그려지지 않게 된다.
	ForEachChildBFS(StateFlag::Render | StateFlag::RenderTexture, [render](UIComponent* component) {
		component->ImplementRender(render);
		});
}

void UIComponent::SetStateFlag(StateFlag::Type flag, bool enabled) noexcept
{
	m_stateFlag = enabled ? (m_stateFlag | flag) : (m_stateFlag & ~flag);

	if (!enabled) return;
	if (flag == StateFlag::Render) SetStateFlag(StateFlag::RenderTexture, false); //랜더면 랜더텍스쳐를 비활성화
	else if (flag == StateFlag::RenderTexture) SetStateFlag(StateFlag::Render, false); //랜더택스쳐면 랜더를 비활성화
}

//크기를 바꾸면 이 컴포넌트의 자식들의 위치값도 바꿔준다.
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


