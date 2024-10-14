#include "pch.h"
#include "UILayout.h"
#include "UIType.h"

UILayout::~UILayout() = default;
UILayout::UILayout(Rectangle&& area, Vector2&& normalizedPos, Origin&& origin) :
	m_area{ area },
	m_originPoint{},
	m_origin{ Origin::Init },
	m_normalizedPosition{ normalizedPos }
{
	Set(move(origin));
}

XMUINT2 UILayout::GetOriginPoint(Origin origin) const noexcept
{
	switch (origin)
	{
	case Origin::Center: return XMUINT2(m_area.width / 2, m_area.height / 2);
	case Origin::LeftTop: return { 0, 0 };
	}
	return { 0, 0 };
}

void UILayout::Set(Rectangle&& area, Vector2&& normalizedPos, Origin&& origin) noexcept
{
	m_area = area;
	Set(move(origin));
	Set(move(normalizedPos));
}

void UILayout::Set(Rectangle&& area) noexcept
{
	m_area = area;
	Set(move(m_origin));
}

void UILayout::Set(Vector2&& normalPos) noexcept
{
	m_normalizedPosition = normalPos;
}

void UILayout::Set(Origin&& origin) noexcept
{
	m_originPoint = GetOriginPoint(origin);
	m_origin = origin;
}

void UILayout::Set(Rectangle&& area, Vector2&& normalPos) noexcept
{
	Set(move(area));
	Set(move(normalPos));
}

XMUINT2 UILayout::GetPosition(const Vector2& resolution) const noexcept
{
	Vector2 pos{ resolution * m_normalizedPosition };
	return { static_cast<long>(pos.x) - m_originPoint.x, static_cast<long>(pos.y) - m_originPoint.y };
}