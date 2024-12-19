#include "pch.h"
#include "TransformComponent.h"
#include "UIComponent.h"
#include "JsonOperation.h"
#include "UILayout.h"
#include "../Utility.h"
#include "../HelperClass.h"
#include "../InputManager.h"

TransformComponent::TransformComponent() 
{}

TransformComponent::TransformComponent(unique_ptr<UIComponent> comp, const Vector2& position) :
	m_component{ move(comp) }, m_position{ position }
{}

//TransformComponent::TransformComponent(const TransformComponent& o) :
//	m_component{ o.m_component->Clone() },
//	m_position{ o.m_position }
//{
//}

TransformComponent::TransformComponent(TransformComponent&& o) noexcept :
	m_component{ move(o.m_component) },
	m_position{ move(o.m_position) }
{
}

//TransformComponent& TransformComponent::operator=(const TransformComponent& other)
//{
//	m_component = other.m_component->Clone();
//	m_position = other.m_position;
//
//	return *this;
//}

bool TransformComponent::operator==(const TransformComponent& o) const noexcept
{
	ReturnIfFalse(CompareUniquePtr(m_component, o.m_component));
	ReturnIfFalse(CompareEpsilon(m_position, o.m_position));

	return true;
}

void TransformComponent::SerializeIO(JsonOperation& operation)
{
	operation.Process("Position", m_position);
	operation.Process("Component", m_component);
}

TransformComponent TransformComponent::Clone() const
{
	TransformComponent transCom;
	transCom.m_component = m_component->Clone();
	transCom.m_position = m_position;
	return transCom;
}