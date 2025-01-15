#include "pch.h"
#include "TransformComponent.h"
#include "UILayout.h"
#include "UIComponent.h"
#include "JsonOperation.h"
#include "../Utility.h"

TransformComponent::TransformComponent() 
{
}

TransformComponent::TransformComponent(
	unique_ptr<UIComponent> comp, const XMINT2& relativePos, const Vector2& _ratio) :
	component{ move(comp) }, relativePosition{ relativePos }, ratio{ _ratio }
{}

TransformComponent::TransformComponent(
	unique_ptr<UIComponent> comp, const XMUINT2& size, const XMINT2& relativePos) :
	component{ move(comp) }
{
	SetRelativePosition(size, relativePos);
}

TransformComponent::TransformComponent(TransformComponent&& o) noexcept :
	component{ move(o.component) },
	relativePosition{ move(o.relativePosition) },
	ratio{ move(o.ratio) }
{}

//vector�� ������ ������ ������ ���� ������ �Ǵ� �̵������ڰ� �־�� �Ѵ�. vector�� �̵��� �ؼ� �����ϴ� �˰����� ������ �ֱ� �����̴�.
TransformComponent& TransformComponent::operator=(TransformComponent&& o) noexcept 
{
	if (this == &o) return *this;
	
	component = move(o.component);
	relativePosition = move(o.relativePosition);
	ratio = move(o.ratio);

	return *this;
}

bool TransformComponent::operator==(const TransformComponent& o) const noexcept
{
	ReturnIfFalse(CompareUniquePtr(component, o.component));
	if (relativePosition != o.relativePosition) return false;
	ReturnIfFalse(CompareEpsilon(ratio, o.ratio));
	
	return true;
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
	operation.Process("Component", component);
}