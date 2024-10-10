#include "pch.h"
#include "UILayout.h"
#include "UIType.h"

UILayout::~UILayout() = default;
UILayout::UILayout()
{}

XMUINT2 UILayout::GetOriginPosition(Origin origin) noexcept
{
	switch (origin)
	{
	case Origin::Center: return XMUINT2(m_area.width / 2, m_area.height / 2);
	case Origin::LeftTop: return XMUINT2(0, 0);
	}
	return XMUINT2(0, 0);
}

void UILayout::SetOrigin(Origin origin)
{
	m_originPosition = GetOriginPosition(origin);
}

void UILayout::Set(const Rectangle& area, const Vector2& pos, Origin origin)
{
	m_area = area;
	SetOrigin(origin);
	m_position = pos;
}