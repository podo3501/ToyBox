#include "pch.h"
#include "UIComponent.h"
#include "Shared/Utils/StlExt.h"
#include "Shared/Utils/GeometryExt.h"
#include "Shared/SerializerIO/SerializerIO.h"
#include "../SerializerIO/ClassSerializeIO.h"
#include "Traverser/UITraverser.h"

using namespace UITraverser;

UIComponent::~UIComponent() = default;
UIComponent::UIComponent() :
	m_root{ this },
	m_layout{ XMUINT2{}, Origin::LeftTop }
{}

UIComponent::UIComponent(const string& name, const UILayout& layout) noexcept :
	m_root{ this },
	m_name{ name },
	m_layout{ layout }
{}

//상속받은 곳에서만 복사생성자를 호출할 수 있다.
UIComponent::UIComponent(const UIComponent& other)
{
	m_root = other.m_root;
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

	ReturnIfFalse(EqualComponent(m_root, o.m_root));
	ReturnIfFalse(tie(m_name, m_layout, m_region, m_stateFlag, m_transform, m_renderTraversal) ==
		tie(o.m_name, o.m_layout, o.m_region, o.m_stateFlag, o.m_transform, o.m_renderTraversal));
	ReturnIfFalse(EqualComponent(m_parent, o.m_parent));
	ReturnIfFalse(m_children.size() == o.m_children.size());
	ReturnIfFalse(CompareSeq(m_children, o.m_children));

	return true;
}

void UIComponent::UnlinkAndRefresh() noexcept
{
	m_root = this;
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

bool UIComponent::RecursiveUpdate(const DX::StepTimer& timer, const XMINT2& position) noexcept
{
	if (!HasStateFlag(StateFlag::Update)) return true;

	const auto& startPos = GetTransform(this).GetUpdatedPosition(m_layout, position);
	ReturnIfFalse(ImplementUpdate(timer));
	
	bool result = ranges::all_of(m_children, [this, &timer, &startPos](auto& child) {
		auto childStartPos = startPos + GetTransform(child.get()).GetRelativePosition();
		return child->RecursiveUpdate(timer, childStartPos);
		});
	
	return result;
}

bool UIComponent::ProcessUpdate(const DX::StepTimer& timer) noexcept
{
	return RecursiveUpdate(timer);
}

void UIComponent::ProcessRender(ITextureRender* texRender)
{
	Render(this, texRender);
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

vector<UIComponent*> UIComponent::GetChildren() const noexcept
{
	vector<UIComponent*> componentList;
	componentList.reserve(m_children.size());

	for (const auto& child : m_children)
	{
		if (child)
			componentList.push_back(child.get());
	}

	return componentList;
}

unique_ptr<UIComponent> UIComponent::AttachComponent(unique_ptr<UIComponent> child, const XMINT2& relativePos) noexcept
{
	if (!HasStateFlag(StateFlag::Attach))
		return move(child);

	child->SetParent(this);
	child->m_transform.ChangeRelativePosition(
		m_layout.GetSize(), relativePos);
	m_children.emplace_back(move(child));
	UpdatePositionsManually(true);

	return nullptr;
}

pair<unique_ptr<UIComponent>, UIComponent*> UIComponent::DetachComponent() noexcept
{
	if (!m_parent || !m_parent->HasStateFlag(StateFlag::Detach))
		return {};

	auto& children = m_parent->m_children;
	auto it = std::ranges::find_if(children, [this](auto& c) { return c.get() == this; });
	if (it == children.end())
		return {};

	auto detached = move(*it);
	UIComponent* parent = m_parent;

	children.erase(it);
	detached->UnlinkAndRefresh();

	return { move(detached), parent };
}

bool ChangeSizeX(UIComponent* c, uint32_t v) noexcept { return c->ChangeSize({ v, c->GetSize().y }); }
bool ChangeSizeX(UIComponent* c, const XMUINT2& s) noexcept { return ChangeSizeX(c, s.x); }
bool ChangeSizeY(UIComponent* c, uint32_t v) noexcept { return c->ChangeSize({ c->GetSize().x, v }); }
bool ChangeSizeY(UIComponent* c, const XMUINT2& s) noexcept { return ChangeSizeY(c, s.y); }
