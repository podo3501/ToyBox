#include "pch.h"
#include "UITransform.h"
#include "UILayout.h"
#include "../JsonOperation/JsonOperation.h"
#include "../../Utility.h"

UITransform::UITransform() = default;
bool UITransform::operator==(const UITransform& o) const noexcept
{
	if (m_relativePosition != o.m_relativePosition) return false;
	ReturnIfFalse(CompareEpsilon(m_ratio, o.m_ratio));
	
	return true;
}

void UITransform::Clear() noexcept
{
	m_ratio = {};
	m_absolutePosition = {};
	m_relativePosition = {};
}

XMINT2 UITransform::GetUpdatedPosition(const UILayout& layout, const XMINT2& parentPos) noexcept
{
	m_absolutePosition = layout.GetPosition(parentPos);
	return m_absolutePosition;
}

void UITransform::SetRelativePosition(const XMUINT2& size, const XMINT2& relativePos) noexcept
{
	m_ratio = { 
		static_cast<float>(relativePos.x) / static_cast<float>(size.x),
		static_cast<float>(relativePos.y) / static_cast<float>(size.y) };
	m_relativePosition = relativePos;
}

void UITransform::AdjustPosition(const XMUINT2& size) noexcept
{
	m_relativePosition = {
		static_cast<int32_t>(static_cast<float>(size.x) * m_ratio.x),
		static_cast<int32_t>(static_cast<float>(size.y) * m_ratio.y) };
}

void UITransform::SerializeIO(JsonOperation& operation)
{
	operation.Process("Ratio", m_ratio);
	operation.Process("RelativePosition", m_relativePosition);
}