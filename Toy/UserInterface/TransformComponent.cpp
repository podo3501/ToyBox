#include "pch.h"
#include "TransformComponent.h"
#include "JsonHelper.h"
#include "UIComponent.h"
#include "Dialog.h"
#include "JsonOperation.h"

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
	return tie(*m_component, m_position) == tie(*o.m_component, o.m_position);
}

bool TransformComponent::LoadResources(ILoadData* load) 
{ 
	return m_component->LoadResources(load); 
}

bool TransformComponent::Update(const Vector2& position, const Mouse::ButtonStateTracker* tracker) noexcept 
{ 
	return m_component->Update(m_position + position, tracker); 
}

void TransformComponent::SetSelected(bool selected) noexcept 
{ 
	m_selected = selected; 
}

bool TransformComponent::IsSelected() const noexcept 
{ 
	return m_selected; 
}

bool TransformComponent::IsPicking(const Vector2& pos) const noexcept 
{ 
	return m_component->IsPicking(pos); 
}

void TransformComponent::Render(IRender* render) 
{ 
	m_component->Render(render); 
}

void TransformComponent::SetPosition(const Vector2& position) noexcept 
{ 
	m_position = position; 
}

UIComponent* TransformComponent::GetComponent() const noexcept 
{ 
	return m_component.get(); 
}

const string& TransformComponent::GetName() const 
{ 
	return m_component->GetName(); 
}

void to_json(nlohmann::ordered_json& j, const TransformComponent& data)
{
	DataToJson("Position", data.m_position, j);
	ComponentToJson("Type", data.m_component, j);
}

void from_json(const nlohmann::json& j, TransformComponent& data)
{
	DataFromJson("Position", data.m_position, j);
	ComponentFromJson("Type", data.m_component, j);
}

void TransformComponent::SerializeIO(JsonOperation* operation)
{
	operation->Process("Position", m_position);
	operation->Process("Component", m_component);
}