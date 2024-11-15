#include "pch.h"
#include "Property.h"
#include "JsonHelper.h"
#include "UIComponent.h"

Property::Property() :
	m_component{ make_unique<UIComponent>() }
{}

Property::Property(unique_ptr<UIComponent> comp, const Vector2& position) :
	m_component{ move(comp) }, m_position{ position }
{}

Property::Property(const Property& other)
{
	m_component = other.m_component->Clone();
	m_position = other.m_position;
	m_selected = false;
}

bool Property::LoadResources(ILoadData* load) 
{ 
	return m_component->LoadResources(load); 
}

bool Property::Update(const Vector2& position, const Mouse::ButtonStateTracker* tracker) noexcept 
{ 
	return m_component->Update(m_position + position, tracker); 
}

void Property::SetSelected(bool selected) noexcept 
{ 
	m_selected = selected; 
}

bool Property::IsSelected() const noexcept 
{ 
	return m_selected; 
}

bool Property::IsPicking(const Vector2& pos) const noexcept 
{ 
	return m_component->IsPicking(pos); 
}

void Property::Render(IRender* render) 
{ 
	m_component->Render(render); 
}

void Property::SetPosition(const Vector2& position) noexcept 
{ 
	m_position = position; 
}

UIComponent* Property::GetComponent() const noexcept 
{ 
	return m_component.get(); 
}

const string& Property::GetName() const 
{ 
	return m_component->GetName(); 
}

void to_json(nlohmann::ordered_json& j, const Property& data)
{
	DataToJson("Position", data.m_position, j);
	DataToJson("Component", data.m_component, j);
}

void from_json(const nlohmann::json& j, Property& data)
{
	DataFromJson("Position", data.m_position, j);
	DataFromJson("Component", data.m_component, j);
}