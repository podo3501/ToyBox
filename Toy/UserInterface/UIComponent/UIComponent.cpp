#include "pch.h"
#include "UIComponent.h"
#include "Shared/Utils/StlExt.h"
#include "Shared/Utils/GeometryExt.h"
#include "Shared/SerializerIO/SerializerIO.h"
#include "../SerializerIO/ClassSerializeIO.h"

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
	m_renderTraversal = other.m_renderTraversal;
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

	ReturnIfFalse(tie(m_name, m_layout, m_region, m_stateFlag, m_transform, m_renderTraversal) ==
		tie(o.m_name, o.m_layout, o.m_region, o.m_stateFlag, o.m_transform, o.m_renderTraversal));
	ReturnIfFalse(EqualComponent(m_parent, o.m_parent));
	ReturnIfFalse(m_children.size() == o.m_children.size());
	ReturnIfFalse(CompareSeq(m_children, o.m_children));

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

bool UIComponent::BindTextureSourceInfo(TextureResourceBinder* resBinder, ITextureController* texController) noexcept
{
	auto forEachResult = ForEachChildPostUntilFail([resBinder, texController](UIComponent* component) {
		bool result = component->ImplementBindSourceInfo(resBinder, texController);
		AssertMsg(result, "Failed to load texture");
		return result;
		});
	ReturnIfFalse(forEachResult);
	ReturnIfFalse(UpdatePositionsManually());
	return true;
}

UITransform& UIComponent::GetTransform(UIComponent* component)
{
	return component->m_transform;
}

bool UIComponent::UpdatePositionsManually(bool root) noexcept
{
	UIComponent* component = (root) ? GetRoot() : this;
	return component->RecursivePositionUpdate();
}

bool UIComponent::RecursivePositionUpdate(const XMINT2& position) noexcept
{
	const auto& startPos = GetTransform(this).GetUpdatedPosition(m_layout, position);
	ImplementPositionUpdated();

	return ranges::all_of(m_children, [this, &startPos](auto& child) {
		auto childStartPos = startPos + GetTransform(child.get()).GetRelativePosition();
		return child->RecursivePositionUpdate(childStartPos);
		});
}

bool UIComponent::RecursiveUpdate(const DX::StepTimer& timer, const XMINT2& position, bool active) noexcept
{
	if (!HasStateFlag(StateFlag::Update)) return true;

	const auto& startPos = GetTransform(this).GetUpdatedPosition(m_layout, position);
	//if (active)
	//{
		//active = HasStateFlag(StateFlag::ActiveUpdate);
		//if(active) ReturnIfFalse(ImplementUpdate(timer));
	//}
	ReturnIfFalse(ImplementUpdate(timer));
	
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
	ForEachChildToRender([render](UIComponent* component) {
		component->ImplementRender(render);
		return TraverseResult::Continue;
		});
}

void UIComponent::SetChildrenStateFlag(StateFlag::Type flag, bool enabled) noexcept
{
	ranges::for_each(m_children, [this, flag, enabled](auto& child) {
		child->SetStateFlag(flag, enabled);
		});
}

bool UIComponent::ImplementResizeAndAdjustPos(const XMUINT2& size) noexcept
{
	ranges::for_each(m_children, [this, &size](auto& child) {
		GetTransform(child.get()).AdjustPosition(size, HasStateFlag(StateFlag::LockPosOnResize));
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

	ReturnIfFalse(ImplementResizeAndAdjustPos(size));
	return ImplementChangeSize(lockedSize, isForce);
}

bool UIComponent::ChangePosition(size_t index, const XMUINT2& size, const XMINT2& relativePos) noexcept
{
	if (index >= m_children.size()) return false;
	GetTransform(m_children[index].get()).ChangeRelativePosition(size, relativePos);
	return true;
}

string UIComponent::GetMyRegion() const noexcept
{
	UIComponent* regionRoot = GetRegionRoot();
	return regionRoot->GetRegion();
}

bool UIComponent::EnableToolMode(bool enable) noexcept
{
	bool result = ranges::all_of(m_children, [enable](auto& child) {
		return child->EnableToolMode(enable);
		});
	ReturnIfFalse(result);

	if (m_toolMode != enable)
	{
		m_toolMode = enable;
		ReturnIfFalse((enable) ? EnterToolMode() : ExitToolMode());
	}

	return true;
}

void UIComponent::ProcessIO(SerializerIO& serializer)
{
	serializer.Process("Name", m_name);
	serializer.Process("Layout", m_layout);
	serializer.Process("Transform", m_transform);
	serializer.Process("Region", m_region);
	serializer.Process("StateFlag", m_stateFlag);
	serializer.Process("RenderSearch", m_renderTraversal);
	serializer.Process("Children", m_children);
	
	if (serializer.IsWrite()) return;
	ranges::for_each(m_children, [this](auto& child) {
		child->SetParent(this);
		});
}

bool UIComponent::ChangeRelativePosition(const XMINT2& relativePos) noexcept
{
	if (!m_parent) return false;
	m_transform.ChangeRelativePosition(m_parent->GetSize(), relativePos);
	return true;
}

Rectangle UIComponent::GetArea() const noexcept
{
	const XMINT2& curLeftTop = GetLeftTop();
	const XMUINT2& curSize = GetSize();
	return Rectangle(curLeftTop.x, curLeftTop.y, curSize.x, curSize.y);
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


