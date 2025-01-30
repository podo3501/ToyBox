#include "pch.h"
#include "UIComponent.h"
#include "../Utility.h"
#include "../InputManager.h"
#include "TransformComponent.h"
#include "JsonOperation.h"
#include "UIComponentEx.h"

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
	m_enable = other.m_enable;
	m_attachmentState = other.m_attachmentState;
	m_transform = other.m_transform;

	ranges::transform(other.m_children, back_inserter(m_children), [this, &other](const auto& child) {
		auto component = child->Clone();
		component->SetParent(this);
		return move(component);
	});
}

bool UIComponent::operator==(const UIComponent& o) const noexcept
{
	if (GetTypeID() != o.GetTypeID()) return false;

	ReturnIfFalse(tie(m_name, m_layout, m_enable, m_attachmentState, m_transform) ==
		tie(o.m_name, o.m_layout, o.m_enable, m_attachmentState, o.m_transform));
	ReturnIfFalse(EqualComponent(m_parent, o.m_parent));
	ReturnIfFalse(m_children.size() == o.m_children.size());
	ReturnIfFalse(ranges::equal(m_children, o.m_children, [](const auto& lhs, const auto& rhs) {
		return CompareUniquePtr(lhs, rhs); }));

	return true;
}

bool UIComponent::EqualComponent(const UIComponent* lhs, const UIComponent* rhs) const noexcept
{
	if (lhs == nullptr && rhs == nullptr) return true;
	if (lhs == nullptr || rhs == nullptr) return false;
	if (lhs->m_name != rhs->m_name) return false;

	return true;
}

UIComponent::UIComponent(UIComponent&& o) noexcept :
	m_name{ move(o.m_name) },
	m_layout{ move(o.m_layout) },
	m_transform{ move(o.m_transform) },
	m_children{ move(o.m_children) }
{}

unique_ptr<UIComponent> UIComponent::Clone() const 
{ 
	return CreateClone();
}

bool UIComponent::LoadResources(ILoadData* load)
{
	return ranges::all_of(m_children, [load](const auto& child) {
		return child->LoadResources(load);
		});
}

bool UIComponent::SetDatas(IGetValue* value)
{
	return ranges::all_of(m_children, [value](const auto& child) {
		return child->SetDatas(value);
		});
}

TransformComponent& UIComponent::GetTransform(UIComponent* component)
{
	return component->m_transform;
}

bool UIComponent::ProcessUpdate(const XMINT2& position) noexcept
{
	if (!m_enable) return true;

	ReturnIfFalse(ImplementUpdate(position));

	auto result = ranges::all_of(m_children, [this, &position](auto& child) {
		const auto& curPosition = GetTransform(child.get()).GetPosition(m_isDirty, m_layout, position);
		return child->ProcessUpdate(curPosition);
		});
	m_isDirty = false;

	return result;
}


void UIComponent::ProcessRender(IRender* render)
{
	//9방향 이미지는 같은 레벨인데 9방향 이미지 위에 다른 이미지를 올렸을 경우 BFS가 아니면 밑에 이미지가 올라온다.
	//가장 밑에 레벨이 가장 위에 올라오는데 DFS(Depth First Search)이면 가장 밑에 있는게 가장 나중에 그려지지 않게 된다.
	ForEachChildBFS([render](UIComponent* component) {
		component->ImplementRender(render);
		});
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

bool UIComponent::IsUniqueName(const string& name, UIComponent* self) noexcept
{
	UIComponent* findComponent = UIComponentEx::GetComponent(this, name);
	if (findComponent && findComponent != self) return false;

	return true;
}

void UIComponent::GenerateUniqueName(UIComponent* addable) noexcept
{
	//붙는 쪽도 붙여지는 쪽도 유니크 이름이어야 한다.
	auto isNotUnique = [this, addable](const string& name) {
		return !(IsUniqueName(name, addable) && addable->IsUniqueName(name, addable));
		};

	int n = 0;
	string baseName = EnumToString(addable->GetTypeID()) + "_";
	string curName{};
	do {
		curName = baseName + to_string(n++);
	} while (isNotUnique(curName));
	addable->Rename(curName);

	for (auto& child : addable->m_children)
		GenerateUniqueName(child.get());
}

unique_ptr<UIComponent> UIComponent::AttachComponent(unique_ptr<UIComponent> component, const XMINT2& relativePos) noexcept
{
	if (!IsAttachable()) return component;

	GenerateUniqueName(component.get());
	component->SetParent(this);
	component->m_transform.SetRelativePosition(m_layout.GetSize(), relativePos); //부모 사이즈와 나의 위치를 비교해야 상대적인 위치값을 구할 수 있다.
	m_children.emplace_back(move(component));
	MarkDirty();

	return nullptr;
}

unique_ptr<UIComponent> UIComponent::DetachComponent(UIComponent* detachComponent) noexcept
{
	auto find = ranges::find_if(m_children, [detachComponent](auto& child) {
		return (child.get() == detachComponent);
		});
	if (find == m_children.end()) return nullptr;

	unique_ptr<UIComponent> detachedComponent = move(*find);
	m_children.erase(find);
	
	detachedComponent->m_parent = nullptr;
	detachedComponent->m_transform.Clear();
	detachedComponent->MarkDirty();
	detachedComponent->ProcessUpdate({});

	return detachedComponent;
}

pair<unique_ptr<UIComponent>, UIComponent*> UIComponent::DetachComponent() noexcept
{
	if (!IsDetachable()) return {};
	if (!m_parent) return {};

	UIComponent* parent = m_parent; //DetachComponent를 하면 parent가 nullptr로 셋팅된다.
	return { move(m_parent->DetachComponent(this)), parent };
}

bool UIComponent::Rename(const string& name) noexcept
{
	if (m_name == name) return true;

	ReturnIfFalse(IsUniqueName(name, this));
	m_name = name;

	return true;
}

void UIComponent::SerializeIO(JsonOperation& operation)
{
	operation.Process("Name", m_name);
	operation.Process("Layout", m_layout);
	operation.Process("Transform", m_transform);
	operation.Process("Enable", m_enable);
	operation.Process("AttachmentState", m_attachmentState);
	operation.Process("Children", m_children);
	
	if (operation.IsWrite()) return;
	ranges::for_each(m_children, [this](auto& child) {
		child->SetParent(this);
		});
}

void UIComponent::MarkDirty() noexcept
{
	if (m_isDirty) return;
	
	m_isDirty = true;
	ranges::for_each(m_children, [](auto& child) {
		child->MarkDirty();
		});
}

XMUINT2 UIComponent::GetTotalChildSize() const noexcept
{
	const Rectangle& totalRectangle = GetTotalChildSize(this);
	return { 
		static_cast<uint32_t>(totalRectangle.width), 
		static_cast<uint32_t>(totalRectangle.height) };
}

Rectangle UIComponent::GetTotalChildSize(const UIComponent* component) const noexcept
{
	if (component == nullptr) return {};

	Rectangle rect{ GetRectangle(component) }; //초기값을 지정하지 않으면 0, 0 부터 시작하는 큰 사각형이 union된다.
	component->ForEachChildConst([&rect](const UIComponent* child) {
		const auto& curRect = GetRectangle(child);
		rect = Rectangle::Union(rect, curRect);
		});

	return rect;
}

optional<XMINT2> UIComponent::GetRelativePosition() const noexcept
{
	return m_transform.GetRelativePosition();
}

bool UIComponent::SetRelativePosition(const XMINT2& relativePos) noexcept
{
	if (!m_parent) return false;
	m_transform.SetRelativePosition(GetSize(m_parent), relativePos);
	m_parent->MarkDirty();
	return true;
}

XMINT2 UIComponent::GetPosition() const noexcept
{
	return GetPositionByLayout(m_transform.absolutePosition);
}

XMINT2 UIComponent::GetPositionByLayout(const XMINT2& position) const noexcept
{
	return position + m_layout.GetPosition();
}

UIComponent* UIComponent::GetRoot() noexcept
{
	UIComponent* current = this;
	while (current->m_parent != nullptr)
		current = current->m_parent;
	
	return current;
}

void UIComponent::GetComponents(const XMINT2& pos, vector<UIComponent*>& outList) noexcept
{
	ForEachChildBFS([&](UIComponent* component) {
		const auto& curPosition = pos - GetTransform(component).absolutePosition;
		if (component->IsArea(curPosition)) 
			outList.push_back(component);
		});
}

UIComponent* UIComponent::GetChildComponent(size_t index) const noexcept
{
	if (index >= m_children.size()) return nullptr;
	return m_children[index].get();
}

vector<UIComponent*> UIComponent::GetComponents() const noexcept
{
	vector<UIComponent*> componentList;
	ranges::transform(m_children, back_inserter(componentList), [](const auto& child) {
		return child.get();
		});

	return componentList;
}

////////////////////////////////////////////////////////

XMINT2 UIComponent::GetAbsolutePosition() const noexcept //나중에 update에 인자로 들어가는 값이기 때문에 삭제될 함수이다.
{
	return m_transform.absolutePosition;
}

void UIComponent::ForEachChild(function<void(UIComponent*)> func) noexcept
{
	func(this);
	for (auto& child : m_children) 
	{
		if (child)
			child->ForEachChild(func);
	}
}

void UIComponent::ForEachChildConst(function<void(const UIComponent*)> func) const noexcept
{
	func(this);
	for (const auto& child : m_children)
	{
		if (child)
			child->ForEachChildConst(func);
	}
}

void UIComponent::ForEachChildBFS(std::function<void(UIComponent*)> func) noexcept
{
	queue<UIComponent*> queue;
	queue.push(this);

	while (!queue.empty())
	{
		UIComponent* current = queue.front();
		queue.pop();

		if (!current->m_enable) continue;

		func(current);

		for (const auto& child : current->m_children)
		{
			if (child)
				queue.push(child.get());
		}
	}
}



