#include "pch.h"
#include "UILayout.h"
#include "UIType.h"

UILayout::~UILayout() = default;
UILayout::UILayout() :
	m_origin{ Origin::Init }
{}

XMUINT2 UILayout::GetOriginPoint(Origin origin) const noexcept
{
	switch (origin)
	{
	case Origin::Center: return XMUINT2(m_area.width / 2, m_area.height / 2);
	case Origin::LeftTop: return { 0, 0 };
	}
	return { 0, 0 };
}

void UILayout::SetOrigin(Origin origin) noexcept
{
	m_originPoint = GetOriginPoint(origin);
	m_origin = origin;
}

void UILayout::ChangeArea(const Rectangle& area) noexcept
{ 
	m_area = area;
	SetOrigin(m_origin);	//area가 변경되어서 새로 계산해 준다.
}

void UILayout::Set(const Rectangle& area, const Vector2& normalizedPos, Origin origin) noexcept
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