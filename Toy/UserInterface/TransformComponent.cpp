#include "pch.h"
#include "TransformComponent.h"
#include "JsonHelper.h"
#include "UIComponent.h"

TransformComponent::TransformComponent() :
	m_component{ make_unique<UIComponent>() }
{}

TransformComponent::TransformComponent(unique_ptr<UIComponent> comp, const Vector2& position) :
	m_component{ move(comp) }, m_position{ position }
{}

TransformComponent::TransformComponent(const TransformComponent& other)
{
	m_component = std::make_unique<UIComponent>(*other.m_component);
	m_position = other.m_position;
}

TransformComponent& TransformComponent::operator=(const TransformComponent& other)
{
	m_component = std::make_unique<UIComponent>(*other.m_component);
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
	DataToJson("Component", data.m_component, j);
}

void from_json(const nlohmann::json& j, TransformComponent& data)
{
	DataFromJson("Position", data.m_position, j);
	DataFromJson("Component", data.m_component, j);
}