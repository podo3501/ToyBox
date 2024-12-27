#include "pch.h"
#include "UIComponent.h"
#include "UIType.h"
#include "../Utility.h"
#include "../HelperClass.h"
#include "../InputManager.h"
#include "TransformComponent.h"
#include "JsonOperation.h"

UIComponent::~UIComponent() = default;
UIComponent::UIComponent() :
	m_name{},
	m_layout{ XMUINT2{}, Origin::LeftTop }
{}

UIComponent::UIComponent(const string& name, const XMUINT2& size) :
	m_name{ name },
	m_layout{ size, Origin::LeftTop }
{}

//상속받은 곳에서만 복사생성자를 호출할 수 있다.
UIComponent::UIComponent(const UIComponent& other)
{
	m_name = other.m_name;
	m_layout = other.m_layout;
	ranges::transform(other.m_components, back_inserter(m_components), [this](const auto& transCom) {
		auto component = transCom.component->Clone();
		component->SetParent(this);
		return move(TransformComponent(move(component), transCom.position));
		});
	m_enable = other.m_enable;
}

string UIComponent::GetType() const { return string(typeid(*this).name()); }

bool UIComponent::operator==(const UIComponent& o) const noexcept
{
	if (GetType() != o.GetType()) return false;

	ReturnIfFalse(tie(m_name, m_layout, m_components) == 
		tie(o.m_name, o.m_layout, o.m_components));
	ReturnIfFalse(EqualComponent(m_parent, o.m_parent));

	return true;
}

bool UIComponent::EqualComponent(const UIComponent* lhs, const UIComponent* rhs) const noexcept
{
	if (lhs == nullptr && rhs == nullptr) return true;
	if (lhs == nullptr || rhs == nullptr) return false;

	if (lhs->GetName() != rhs->GetName()) return false;

	return true;
}

UIComponent::UIComponent(UIComponent&& o) noexcept :
	m_name{ move(o.m_name) },
	m_layout{ move(o.m_layout) },
	m_components{ move(o.m_components) }
{}

unique_ptr<UIComponent> UIComponent::Clone() const 
{ 
	auto clone = CreateClone();
	clone->SetName(clone->GetName() + "_clone");
	return clone;
}

bool UIComponent::LoadResources(ILoadData* load)
{
	return ranges::all_of(m_components, [load](const auto& transComp) {
		return transComp.component->LoadResources(load);
		});
}

bool UIComponent::SetDatas(IGetValue* value)
{
	return ranges::all_of(m_components, [value](const auto& transCom) {
		return transCom.component->SetDatas(value);
		});
}

bool UIComponent::ProcessUpdate(const XMINT2& position, InputManager* inputManager) noexcept
{
	if (!m_enable) return true;

	Update(position, inputManager);

	auto result = ranges::all_of(m_components, [this, &position, inputManager](auto& transCom) {
		const auto& curPosition = transCom.GetPosition(m_isDirty, m_layout, position);
		if (inputManager)
			inputManager->GetMouse()->SetOffset(curPosition);
		return transCom.component->ProcessUpdate(curPosition, inputManager);
		});
	m_isDirty = false;

	return result;
}

void UIComponent::ProcessRender(IRender* render)
{
	if (!m_enable) 
		return;

	Render(render);

	ranges::for_each(m_components, [render](const auto& transCom) {
		transCom.component->ProcessRender(render);
		});
}

void UIComponent::SetChildPosition(const string& name, const Vector2& position) noexcept
{
	auto transComponent = const_cast<TransformComponent*>(FindTransformComponent(name));
	if (transComponent == nullptr) return;

	transComponent->position = position;
	MarkDirty();
}

bool UIComponent::ChangePosition(int index, const Vector2& pos) noexcept
{
	if (index >= m_components.size()) return false;
	m_components[index].position = pos;
	MarkDirty();

	return true;
}

void UIComponent::AddComponent(unique_ptr<UIComponent>&& component, const Vector2& pos)
{
	component->SetParent(this);
	auto transComponent = TransformComponent(move(component), pos);
	m_components.emplace_back(move(transComponent));
	MarkDirty();
}

void UIComponent::SerializeIO(JsonOperation& operation)
{
	operation.Process("Name", m_name);
	operation.Process("Layout", m_layout);
	operation.Process("Components", m_components);
	
	if (operation.IsWrite()) return;
	ranges::for_each(m_components, [this](auto& transComponent) {
		transComponent.component->SetParent(this);
		});
}

void UIComponent::MarkDirty() noexcept
{
	if (m_isDirty) return;
	
	m_isDirty = true;
	ranges::for_each(m_components, [](auto& transComponent) {
		transComponent.component->MarkDirty();
		});
}

XMINT2 UIComponent::GetComponentPosition(const UIComponent* component) const noexcept
{
	UIComponent* parent = component->m_parent;
	if (parent == nullptr) return { 0, 0 };

	auto transComponent = parent->FindTransformComponent(component);
	assert(transComponent);	//없다는 것은 parent가 잘못 돼 있다던지, 이름이 잘못 돼 있다던지 있어서는 안되는 일이다.
	return parent->GetLayout().GetPosition(transComponent->position);
}

XMINT2 UIComponent::GetPosition() const noexcept
{
	XMINT2 parentPosition{ 0, 0 };
	if (m_parent)
	{
		auto transformComponent = m_parent->FindTransformComponent(this);
		parentPosition = transformComponent->realPosition;
	}

	return GetPositionByLayout(parentPosition);
}

Rectangle UIComponent::GetRectangle() const noexcept
{
	XMINT2 curPosition = GetPosition();
	return Rectangle(curPosition.x, curPosition.y, GetSize().x, GetSize().y);
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

void UIComponent::RefreshPosition() noexcept
{
	UIComponent* component = GetRoot();
	component->ProcessUpdate({}, nullptr);
}

void UIComponent::GetComponents(const XMINT2& pos, vector<UIComponent*>& outList) noexcept
{
	if (IsArea(pos))
		outList.push_back(this);

	ranges::for_each(m_components, [this, &pos, &outList](auto& transCom) {
		const auto& curPosition = pos - m_layout.GetPosition(transCom.position);
		transCom.component->GetComponents(curPosition, outList);
		});
}

UIComponent* UIComponent::GetComponent(const string& name) const noexcept
{
	if (GetName() == name)
		return const_cast<UIComponent*>(this);

	for (const auto& transComponent : m_components)
	{
		UIComponent* find = transComponent.component->GetComponent(name);
		if (find) return find;
	}
	
	return nullptr;
}

vector<UIComponent*> UIComponent::GetComponents() const noexcept
{
	vector<UIComponent*> componentList;
	ranges::transform(m_components, back_inserter(componentList), [](const auto& transCom) {
		return transCom.component.get();
		});

	return componentList;
}

const TransformComponent* UIComponent::FindTransformComponent(const string& name) const noexcept
{
	auto find = ranges::find_if(m_components, [&name](const auto& transComp) {
		return transComp.component->GetName() == name;
		});

	if (find == m_components.end()) return nullptr;
	return &(*find);
}

const TransformComponent* UIComponent::FindTransformComponent(const UIComponent* component) const noexcept
{
	auto find = ranges::find_if(m_components, [component](const auto& transComp) {
		return transComp.component.get() == component;
		});

	if (find == m_components.end()) return nullptr;
	return &(*find);
}



