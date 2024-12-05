#include "pch.h"
#include "UILayout.h"
#include "UIType.h"
#include "../Config.h"
#include "../Utility.h"
#include "JsonOperation.h"

using json = nlohmann::json;
using ordered_json = nlohmann::ordered_json;

UILayout::UILayout() = default;
UILayout::~UILayout() = default;
Origin GetOrigin(const string& strOrigin)
{
	if (strOrigin == "LeftTop") return Origin::LeftTop;
	if (strOrigin == "Center") return Origin::Center;

	return Origin::Init;
}

UILayout& UILayout::operator=(const UILayout& other) = default;

string UILayout::GetType() const { return string(typeid(UILayout).name()); }

UILayout::UILayout(const json& data)
{
	const auto& rect = data["Area"];
	m_area.x = rect["x"];
	m_area.y = rect["y"];
	m_area.width = rect["width"];
	m_area.height = rect["height"];

	m_origin = GetOrigin(data["Origin"]);
}

UILayout::UILayout(const Rectangle& area, const Origin& origin) :
	m_area{ area },
	m_originPoint{},
	m_origin{ Origin::Init }
{
	Set(origin);
}

bool UILayout::operator==(const UILayout& o) const noexcept
{
	return tie(m_area, m_origin) == tie(o.m_area, o.m_origin);
}

XMINT2 UILayout::GetOriginPoint(Origin origin) const noexcept
{
	switch (origin)
	{
	case Origin::Center: return XMINT2(m_area.width / 2, m_area.height / 2);
	case Origin::LeftTop: return { 0, 0 };
	}
	return { 0, 0 };
}

void UILayout::Set(const Rectangle& area, const Origin& origin) noexcept
{
	m_area = area;
	Set(origin);
}

void UILayout::Set(const Rectangle& area) noexcept
{
	m_area = area;
	Set(m_origin);
}

void UILayout::Set(const Origin& origin) noexcept
{
	m_originPoint = GetOriginPoint(origin);
	m_origin = origin;
}

XMINT2 TransformVectorByRect(const Rectangle& rect, const Vector2& vec)
{
	return XMINT2(rect.x + static_cast<uint32_t>(vec.x * rect.width), rect.y + static_cast<uint32_t>(vec.y * rect.height));
}

XMINT2 UILayout::GetPosition(const Vector2& position) const noexcept
{
	//return GetResolution() * position - m_originPoint;
	return TransformVectorByRect(m_area, position) - m_originPoint;
}

bool UILayout::IsArea(const XMINT2& pos) const noexcept
{
	return m_area.Contains(pos.x, pos.y);
}

void UILayout::Union(const Rectangle& area) noexcept
{
	m_area = Rectangle::Union(m_area, area);
}

void UILayout::SerializeIO(JsonOperation& operation)
{
	operation.Process("Area", m_area);
	operation.Process("Origin", m_origin);
	m_originPoint = GetOriginPoint(m_origin);	//load 했을때에는 m_originPoint를 계산해준다.
}