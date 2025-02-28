#include "pch.h"
#include "UILayout.h"
#include "UIType.h"
#include "../Config.h"
#include "../Utility.h"
#include "JsonOperation.h"

UILayout::~UILayout() = default;
UILayout::UILayout() :
	UILayout(XMUINT2{}, Origin::LeftTop) // 기본 생성자 통합
{}

UILayout::UILayout(const XMUINT2& size, const Origin& origin) :
	m_size{ size },
	m_originPoint{ GetOriginPoint(origin) },
	m_origin{ origin }
{}

UILayout& UILayout::operator=(const UILayout& other) = default;

string UILayout::GetType() const { return string(typeid(UILayout).name()); }

bool UILayout::operator==(const UILayout& o) const noexcept
{
	return tie(m_size, m_origin) == tie(o.m_size, o.m_origin);
}

XMINT2 UILayout::GetOriginPoint(Origin origin) const noexcept
{
	switch (origin)
	{
	case Origin::Center: return XMINT2(m_size.x / 2, m_size.y / 2);
	case Origin::LeftTop: return { 0, 0 };
	}
	return { 0, 0 };
}

void UILayout::Set(const XMUINT2& size, const Origin& origin) noexcept
{
	m_size = size;
	Set(origin);
}

void UILayout::Set(const XMUINT2& size) noexcept
{
    Set(size, m_origin);
}

void UILayout::Set(const Origin& origin) noexcept
{
    m_origin = origin;
    m_originPoint = GetOriginPoint(origin);
}

static inline XMINT2 TransformVectorByRect(const XMUINT2& size, const Vector2& vec)
{
	return XMINT2(
		static_cast<int32_t>(vec.x * static_cast<float>(size.x)),
		static_cast<int32_t>(vec.y * static_cast<float>(size.y))
	);
}

//XMINT2 UILayout::GetPosition(const Vector2& position) const noexcept
//{
//	return TransformVectorByRect(m_size, position) - m_originPoint;
//}

XMINT2 UILayout::GetPosition(const XMINT2& relativePosition) const noexcept
{
	return relativePosition - m_originPoint;
}

void UILayout::SerializeIO(JsonOperation& operation)
{
	operation.Process("Size", m_size);
	operation.Process("Origin", m_origin);

	if (operation.IsWrite()) return;
	m_originPoint = GetOriginPoint(m_origin);	//load 했을때에는 m_originPoint를 계산해준다.
}