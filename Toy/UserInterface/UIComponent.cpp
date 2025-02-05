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
	m_enable = other.m_enable;
	m_bRegion = other.m_bRegion;
	m_region = other.m_region;
	m_attachmentState = other.m_attachmentState;
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

	ReturnIfFalse(tie(m_name, m_layout, m_enable, m_bRegion, m_region, m_attachmentState, m_transform) ==
		tie(o.m_name, o.m_layout, o.m_enable, o.m_bRegion, o.m_region, m_attachmentState, o.m_transform));
	ReturnIfFalse(EqualComponent(m_parent, o.m_parent));
	ReturnIfFalse(m_children.size() == o.m_children.size());
	ReturnIfFalse(ranges::equal(m_children, o.m_children, [](const auto& lhs, const auto& rhs) {
		return CompareUniquePtr(lhs, rhs); }));

	return true;
}

UIComponent::UIComponent(UIComponent&& o) noexcept :
	m_name{ move(o.m_name) },
	m_layout{ move(o.m_layout) },
	m_bRegion{ move(o.m_bRegion) },
	m_region{ move(o.m_region) },
	m_transform{ move(o.m_transform) },
	m_children{ move(o.m_children) }
{}

unique_ptr<UIComponent> UIComponent::Clone() const 
{ 
	if (!IsDetachable()) return nullptr; //Detach�� �ȵǴ� ���� ������Ʈ�� �μ��̱� ������ Ŭ���ص� ������ ����.

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

UITransform& UIComponent::GetTransform(UIComponent* component)
{
	return component->m_transform;
}

bool UIComponent::ProcessUpdate(const XMINT2& position, bool activeUpdate) noexcept
{
	if (!m_enable) return true;

	ReturnIfFalse(ImplementUpdatePosition(position));
	if(activeUpdate) //�������� ���콺�� �ö󰡼� ���º�ȭ�� �����, �۾��� �ȵȴ�.
		ReturnIfFalse(ImplementActiveUpdate(position));

	auto result = ranges::all_of(m_children, [this, &position, activeUpdate](auto& child) {
		const auto& curPosition = GetTransform(child.get()).GetPosition(m_isDirty, m_layout, position);
		return child->ProcessUpdate(curPosition, activeUpdate);
		});
	m_isDirty = false;

	return result;
}

void UIComponent::ProcessRender(IRender* render)
{
	//9���� �̹����� ���� �����ε� 9���� �̹��� ���� �ٸ� �̹����� �÷��� ��� BFS�� �ƴϸ� �ؿ� �̹����� �ö�´�.
	//���� �ؿ� ������ ���� ���� �ö���µ� DFS(Depth First Search)�̸� ���� �ؿ� �ִ°� ���� ���߿� �׷����� �ʰ� �ȴ�.
	ForEachChildBFS([render](UIComponent* component) {
		component->ImplementRender(render);
		});
}

//ũ�⸦ �ٲٸ� �� ������Ʈ�� �ڽĵ��� ��ġ���� �ٲ��ش�.
void UIComponent::ChangeSize(const XMUINT2& size) noexcept
{
	ranges::for_each(m_children, [this, &size](auto& child) {
		GetTransform(child.get()).AdjustPosition(size);
		});

	ApplySize(size);
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

bool UIComponent::ChangePosition(int index, const XMUINT2& size, const XMINT2& relativePos) noexcept
{
	if (index >= m_children.size()) return false;
	
	GetTransform(m_children[index].get()).SetRelativePosition(size, relativePos);
	MarkDirty();

	return true;
}

bool UIComponent::IsUniqueName(const string& name, UIComponent* self) noexcept
{
	//�ڽ��� RegionEntry�̸� �Ҽ��� �� ���� Region�� �ֱ� ������ ������ �˻��ϵ��� �Ѵ�.
	bool isRegionEntry = (self->GetBRegion() && self->m_parent);
	auto& componentEx = isRegionEntry ? GetUIComponentEx() : self->GetUIComponentEx();
	//auto& componentEx = self->GetRegion() ? GetUIComponentEx() : self->GetUIComponentEx();
	//auto& componentEx = GetUIComponentEx();
	UIComponent* findComponent = componentEx.GetComponent(name);
	
	if (findComponent && findComponent != self) return false;

	return true;
}

//���� �̸��� _���� �� �پ������� �̸��� _�� ���� �ƴϸ� Ÿ���̸���_�� �ٿ��� ����
static string GetBaseName(const string& name, ComponentID id)
{
	if (name.empty())
		return EnumToString(id) + "_";
	
	size_t pos = name.rfind('_');
	if (pos == string::npos)
		return name + "_";

	string baseName = name.substr(0, pos + 1);
	string afterUnderline = name.substr(pos + 1);
	if (ranges::all_of(afterUnderline, ::isdigit)) //_xx xx�� ���� ���ڶ��
		return baseName;

	return name + "_";
}

void UIComponent::GenerateUniqueName(UIComponent* addable) noexcept
{
	//�ٴ� �ʵ� �ٿ����� �ʵ� ����ũ �̸��̾�� �Ѵ�.
	auto isNotUnique = [this, addable](const string& name) {
		return !(IsUniqueName(name, addable) && addable->IsUniqueName(name, addable));
		};

	const string& addableName = addable->m_name;
	if (!addableName.empty() && !isNotUnique(addableName)) //���� �̸��� ����ũ �ϸ� �ٲ��� �ʴ´�.
		return; 

	int n = 0;
	auto baseName = GetBaseName(addableName, addable->GetTypeID());
	string curName{};
	do {
		curName = baseName + to_string(n++);
	} while (isNotUnique(curName));

	addable->Rename(curName);

	for (auto& child : addable->m_children)
		GenerateUniqueName(child.get());
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
	operation.Process("BRegion", m_bRegion);
	operation.Process("Region", m_region);
	operation.Process("AttachmentState", m_attachmentState);
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

void UIComponent::ForEachChild(function<void(UIComponent*)> func) noexcept
{
	func(this);
	for (auto& child : m_children) 
	{
		if (child)
			child->ForEachChild(func);
	}
}

void UIComponent::ForEachChildBool(function<bool(UIComponent*)> func) noexcept
{
	if (!func(this))
		return;

	for (auto& child : m_children)
	{
		if (child)
			child->ForEachChildBool(func);
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


