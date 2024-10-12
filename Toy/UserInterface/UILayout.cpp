#include "pch.h"
#include "UILayout.h"
#include "UIType.h"

UILayout::~UILayout() = default;
UILayout::UILayout()
{}

XMUINT2 UILayout::GetOriginPoint(Origin origin) noexcept
{
	switch (origin)
	{
	case Origin::Center: return XMUINT2(m_area.width / 2, m_area.height / 2);
	case Origin::LeftTop: return { 0, 0 };
	}
	return { 0, 0 };
}

void UILayout::SetOrigin(Origin origin)
{
	m_originPoint = GetOriginPoint(origin);
}

void UILayout::Set(const Rectangle& area, const Vector2& normalizedPos, Origin origin)
{
	m_area = area;
	SetOrigin(origin);
	m_normalizedPosition = normalizedPos;
}

XMUINT2 UILayout::GetPosition(const Vector2& resolution) const noexcept
{
	Vector2 pos{ resolution * m_normalizedPosition };
	return { static_cast<long>(pos.x) - m_originPoint.x, static_cast<long>(pos.y) - m_originPoint.y };
}