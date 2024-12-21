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
	ranges::transform(other.m_components, back_inserter(m_components), [](const auto& transCom) {
		return move(TransformComponent(transCom.component->Clone(), transCom.position));
		});
}

string UIComponent::GetType() const { return string(typeid(*this).name()); }

bool UIComponent::operator==(const UIComponent& o) const noexcept
{
	if (GetType() != o.GetType()) return false;

	ReturnIfFalse(tie(m_name, m_layout, m_components) == tie(o.m_name, o.m_layout, o.m_components));

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

	return ranges::all_of(m_components, [this, &position, inputManager](const auto& transCom) {
		const auto& curPosition = m_layout.GetPosition(transCom.position) + position;
		if (inputManager)
			inputManager->GetMouse()->SetOffset(curPosition);
		return transCom.component->ProcessUpdate(curPosition, inputManager);
		});
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
}

bool UIComponent::ChangePosition(int index, const Vector2& pos) noexcept
{
	if (index >= m_components.size()) return false;
	m_components[index].position = pos;

	return true;
}

void UIComponent::AddComponent(unique_ptr<UIComponent>&& component, const Vector2& pos)
{
	component->SetParent(this);
	auto transComponent = TransformComponent(move(component), pos);
	m_components.emplace_back(move(transComponent));
}

void UIComponent::SerializeIO(JsonOperation& operation)
{
	operation.Process("Name", m_name);
	operation.Process("Layout", m_layout);
	operation.Process("Enable", m_enable);
	operation.Process("Components", m_components);
	
	if (operation.IsWrite()) return;
	//parent를 다시 이어준다.
}

XMINT2 UIComponent::GetPositionRecursive(const UIComponent* component) const noexcept
{
	if (component == nullptr || component->m_parent == nullptr) return { 0, 0 };

	return GetComponentPosition(component) + GetPositionRecursive(component->m_parent);
}

XMINT2 UIComponent::GetComponentPosition(const UIComponent* component) const noexcept
{
	UIComponent* parent = component->m_parent;
	if (parent == nullptr) return { 0, 0 };

	auto transComponent = parent->FindTransformComponent(component->GetName());
	assert(transComponent);	//없다는 것은 parent가 잘못 돼 있다던지, 이름이 잘못 돼 있다던지 있어서는 안되는 일이다.
	return parent->GetLayout().GetPosition(transComponent->position);
}

XMINT2 UIComponent::GetPosition() const noexcept
{
	return GetPositionByLayout(GetPositionRecursive(this));
}

XMINT2 UIComponent::GetPositionByLayout(const XMINT2& position) const noexcept
{
	return position + m_layout.GetPosition();
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