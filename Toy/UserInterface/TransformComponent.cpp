#include "pch.h"
#include "TransformComponent.h"
#include "UILayout.h"
#include "UIComponent.h"
#include "JsonOperation.h"
#include "../Utility.h"

TransformComponent::TransformComponent() 
{
}

TransformComponent::TransformComponent(const XMINT2& relativePos, const Vector2& _ratio) :
	relativePosition{ relativePos }, ratio{ _ratio }
{}

TransformComponent::TransformComponent(const XMUINT2& size, const XMINT2& relativePos)
{
	SetRelativePosition(size, relativePos);
}

bool TransformComponent::operator==(const TransformComponent& o) const noexcept
{
	if (relativePosition != o.relativePosition) return false;
	ReturnIfFalse(CompareEpsilon(ratio, o.ratio));
	
	return true;
}

void TransformComponent::Clear() noexcept
{
	absolutePosition = {};
	relativePosition = {};
	ratio = {};
}

const XMINT2& TransformComponent::GetPosition(bool IsDirty, const UILayout& layout, const XMINT2& parentPosition) noexcept
{
	if (!IsDirty) return absolutePosition;
	absolutePosition = layout.GetPosition(relativePosition) + parentPosition;

	return absolutePosition;
}

void TransformComponent::SetRelativePosition(const XMUINT2& size, const XMINT2& relativePos) noexcept
{
	ratio = {static_cast<float>(relativePos.x) / static_cast<float>(size.x),
		static_cast<float>(relativePos.y) / static_cast<float>(size.y) };
	relativePosition = relativePos;
}

void TransformComponent::AdjustPosition(const XMUINT2& size) noexcept
{
	relativePosition = { 
		static_cast<int32_t>(static_cast<float>(size.x) * ratio.x),
		static_cast<int32_t>(static_cast<float>(size.y) * ratio.y) };
}

void TransformComponent::SerializeIO(JsonOperation& operation)
{
	operation.Process("RelativePosition", relativePosition);
	operation.Process("Ratio", ratio);
	//operation.Process("Component", component);
}