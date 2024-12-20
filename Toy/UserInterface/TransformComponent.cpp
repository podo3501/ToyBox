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
	component{ move(comp) }, position{ position }
{}

TransformComponent::TransformComponent(TransformComponent&& o) noexcept :
	component{ move(o.component) },
	position{ move(o.position) }
{
}

bool TransformComponent::operator==(const TransformComponent& o) const noexcept
{
	ReturnIfFalse(CompareUniquePtr(component, o.component));
	ReturnIfFalse(CompareEpsilon(position, o.position));

	return true;
}

void TransformComponent::SerializeIO(JsonOperation& operation)
{
	operation.Process("Position", position);
	operation.Process("Component", component);
}