#include "pch.h"
#include "TransformComponent.h"
#include "UILayout.h"
#include "UIComponent.h"
#include "JsonOperation.h"
#include "../Utility.h"

TransformComponent::TransformComponent() 
{}

TransformComponent::TransformComponent(unique_ptr<UIComponent> comp, const XMINT2 relativePos) :
	component{ move(comp) }, relativePosition{ relativePos }
{}

TransformComponent::TransformComponent(TransformComponent&& o) noexcept :
	component{ move(o.component) },
	relativePosition{ move(o.relativePosition) }
{}

bool TransformComponent::operator==(const TransformComponent& o) const noexcept
{
	ReturnIfFalse(CompareUniquePtr(component, o.component));
	if (relativePosition != o.relativePosition) return false;

	return true;
}

XMINT2 TransformComponent::GetPosition(bool IsDirty, const UILayout& layout, const XMINT2& parentPosition) noexcept
{
	if (!IsDirty) return absolutePosition;
	absolutePosition = layout.GetPosition(relativePosition) + parentPosition;

	return absolutePosition;
}

void TransformComponent::SerializeIO(JsonOperation& operation)
{
	operation.Process("RelativePosition", relativePosition);
	operation.Process("Component", component);
}