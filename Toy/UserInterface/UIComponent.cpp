#include "pch.h"
#include "UIComponent.h"
#include "../Utility.h"
#include "../InputManager.h"
#include "JsonOperation.h"
#include "StepTimer.h"
#include "../Config.h"

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
	UpdatePositionsManually();
}

unique_ptr<UIComponent> UIComponent::Clone() const 
{ 
	auto clone = CreateClone();
	clone->UnlinkAndRefresh(); //Clone의 root는 위치값을 초기화 시켜준다.
	return clone;
}

bool UIComponent::BindTextureSourceInfo(TextureSourceBinder* sourceBinder, ITextureController* texController) noexcept
{
	return ForEachChildPostUntilFail([sourceBinder, texController](UIComponent* component) {
		return component->ImplementBindSourceInfo(sourceBinder, texController);
		});
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

bool UIComponent::UpdatePositionsManually(bool root) noexcept
{
	DX::StepTimer dummyTimer; //임시타이머이다. 이 타이머는 사용하지 않는다.
	UIComponent* component = (root) ? GetRoot() : this;
	return component->RecursiveUpdate(dummyTimer, {}, false); //ActiveUpdate true를 하면 상태가 바뀐다.
}

bool UIComponent::RecursiveUpdate(const DX::StepTimer& timer, const XMINT2& position, bool active) noexcept
{
	if (!HasStateFlag(StateFlag::Update)) return true;

	const auto& startPos = GetTransform(this).GetUpdatedPosition(m_layout, position);
	if (active)
	{
		active = HasStateFlag(StateFlag::ActiveUpdate);
		if(active) ReturnIfFalse(ImplementUpdate(timer));
	}
	
	bool result = ranges::all_of(m_children, [this, &timer, &startPos, active](auto& child) {
		auto childStartPos = startPos + GetTransform(child.get()).GetRelativePosition();
		return child->RecursiveUpdate(timer, childStartPos, active);
		});
	

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
	ForEachChildBFS(GetRenderFilterFlag(), [render](UIComponent* component) {
		component->ImplementRender(render);
		});
}

void UIComponent::SetChildrenStateFlag(StateFlag::Type flag, bool enabled) noexcept
{
	ranges::for_each(m_children, [this, flag, enabled](auto& child) {
		child->SetStateFlag(flag, enabled);
		});
}

bool UIComponent::ImplementChangeSize(const XMUINT2& size) noexcept
{
	ranges::for_each(m_children, [this, &size](auto& child) {
		GetTransform(child.get()).AdjustPosition(size);
		});

	ApplySize(size);
	return true;
}

//크기를 바꾸면 이 컴포넌트의 자식들의 위치값도 바꿔준다.
bool UIComponent::ChangeSize(const XMUINT2& size, bool isForce) noexcept
{
	XMUINT2 lockedSize{ size };
	const auto& preSize = GetSize();
	if (HasStateFlag(StateFlag::X_SizeLocked)) lockedSize.x = preSize.x;
	if (HasStateFlag(StateFlag::Y_SizeLocked)) lockedSize.y = preSize.y;

	if (!isForce && lockedSize == preSize) return true;

	return ImplementChangeSize(lockedSize);
}

bool UIComponent::ChangePosition(int index, const XMUINT2& size, const XMINT2& relativePos) noexcept
{
	if (index >= m_children.size()) return false;
	GetTransform(m_children[index].get()).SetRelativePosition(size, relativePos);
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

bool UIComponent::SetRelativePosition(const XMINT2& relativePos) noexcept
{
	if (!m_parent) return false;
	m_transform.SetRelativePosition(m_parent->GetSize(), relativePos);
	return true;
}

Rectangle UIComponent::GetArea() const noexcept
{
	const XMINT2& curPosition = GetPosition();
	const XMUINT2& curSize = GetSize();
	return Rectangle(curPosition.x, curPosition.y, curSize.x, curSize.y);
}
   
const XMUINT2& UIComponent::GetSize() const noexcept
{
	return m_layout.GetSize();
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


