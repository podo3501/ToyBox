#include "pch.h"
#include "UILayout.h"
#include "UIType.h"

UILayout::~UILayout() = default;
UILayout::UILayout()
{}

Vector2 GetOriginPosition(const Rectangle& area, Origin origin) noexcept
{
	switch (origin)
	{
	case Origin::Center: return Vector2(static_cast<float>(area.width), static_cast<float>(area.height)) / 2.0f;
	case Origin::LeftTop: return Vector2(0.f, 0.f);
	}
	return Vector2(0.f, 0.f);
}

const Vector2& UILayout::ChangeOrigin(Origin origin)
{
	m_originPosition = GetOriginPosition(m_area, origin);
	return m_originPosition;
}

void UILayout::Set(const Rectangle& area, const Vector2& pos, Origin origin)
{
	m_originPosition = GetOriginPosition(area, origin);
	m_area = area;
	m_position = pos;
}