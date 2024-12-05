#include "pch.h"
#include "TransformComponent.h"
#include "UIComponent.h"
#include "JsonOperation.h"
#include "UILayout.h"
#include "../Utility.h"
#include "../HelperClass.h"

TransformComponent::TransformComponent() 
{}

TransformComponent::TransformComponent(unique_ptr<UIComponent> comp, const Vector2& position) :
	m_component{ move(comp) }, m_position{ position }
{}

TransformComponent::TransformComponent(const TransformComponent& o) :
	m_component{ o.m_component->Clone() },
	m_position{ o.m_position }
{}

TransformComponent::TransformComponent(TransformComponent&& o) noexcept :
	m_component{ move(o.m_component) },
	m_position{ move(o.m_position) }
{}

TransformComponent& TransformComponent::operator=(const TransformComponent& other)
{
	m_component = other.m_component->Clone();
	m_position = other.m_position;

	return *this;
}

bool TransformComponent::operator==(const TransformComponent& o) const noexcept
{
	ReturnIfFalse(CompareUniquePtr(m_component, o.m_component));
	ReturnIfFalse(tie(m_position) == tie(o.m_position));

	return true;
}

bool TransformComponent::LoadResources(ILoadData* load) 
{ 
	return m_component->LoadResources(load); 
}

bool TransformComponent::SetDatas(IGetValue* value)
{
	return m_component->SetDatas(value);
}

bool TransformComponent::Update(const XMINT2& position, MouseTracker* tracker) noexcept
{ 
	return m_component->Update(position, tracker);
}

void TransformComponent::SetSelected(bool selected) noexcept 
{ 
	m_selected = selected; 
}

bool TransformComponent::IsSelected() const noexcept 
{ 
	return m_selected; 
}

bool TransformComponent::IsPicking(const XMINT2& pos) const noexcept 
{ 
	return m_component->IsPicking(pos); 
}

void TransformComponent::GetComponents(const XMINT2& pos, vector<const UIComponent*>& outList) const noexcept
{
	return m_component->GetComponents(pos, outList);
}

void TransformComponent::Render(IRender* render) 
{ 
	m_component->Render(render); 
}

void TransformComponent::SetPosition(const Vector2& position) noexcept 
{ 
	m_position = position; 
}

Vector2 TransformComponent::GetPosition() const noexcept
{
	return m_position;
}

UIComponent* TransformComponent::GetComponent() const noexcept 
{ 
	return m_component.get(); 
}

const string& TransformComponent::GetName() const 
{ 
	return m_component->GetName(); 
}

void TransformComponent::SerializeIO(JsonOperation& operation)
{
	operation.Process("Position", m_position);
	operation.Process("Component", m_component);
}