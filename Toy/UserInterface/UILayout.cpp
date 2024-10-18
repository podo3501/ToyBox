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

Vector2 UILayout::GetOriginPoint(Origin origin) const noexcept
{
	switch (origin)
	{
	case Origin::Center: return Vector2(static_cast<float>(m_area.width) / 2.f, static_cast<float>(m_area.height) / 2.f);
	case Origin::LeftTop: return { 0.f, 0.f };
	}
	return { 0.f, 0.f };
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

Vector2 UILayout::GetPosition(const Vector2& resolution) const noexcept
{
	return ( resolution * m_normalizedPosition ) - m_originPoint;
}