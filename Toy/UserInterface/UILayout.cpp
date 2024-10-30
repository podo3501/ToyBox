#include "pch.h"
#include "UILayout.h"
#include "UIType.h"

UILayout::~UILayout() = default;
UILayout::UILayout(const Rectangle& area, const Vector2& normalizedPos, const Origin& origin) :
	m_area{ area },
	m_originPoint{},
	m_origin{ Origin::Init },
	m_normalizedPosition{ normalizedPos }
{
	Set(origin);
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

void UILayout::Set(const Rectangle& area, const Vector2& normalizedPos, const Origin& origin) noexcept
{
	m_area = area;
	Set(origin);
	Set(normalizedPos);
}

void UILayout::Set(const Rectangle& area) noexcept
{
	m_area = area;
	Set(m_origin);
}

void UILayout::Set(const Vector2& normalPos) noexcept
{
	m_normalizedPosition = normalPos;
}

void UILayout::Set(const Origin& origin) noexcept
{
	m_originPoint = GetOriginPoint(origin);
	m_origin = origin;
}

void UILayout::Set(const Rectangle& area, const Vector2& normalPos) noexcept
{
	Set(area);
	Set(normalPos);
}

Vector2 UILayout::GetPosition(const Vector2& resolution) const noexcept
{
	return ( resolution * m_normalizedPosition ) - m_originPoint;
}

bool UILayout::IsArea(const Vector2& pos) const noexcept
{
	return m_area.Contains(pos);
}