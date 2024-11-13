#include "pch.h"
#include "UILayout.h"
#include "UIType.h"
#include "../Config.h"
#include "JsonHelper.h"

using json = nlohmann::json;
using ordered_json = nlohmann::ordered_json;

UILayout::~UILayout() = default;
Origin GetOrigin(const string& strOrigin)
{
	if (strOrigin == "LeftTop") return Origin::LeftTop;
	if (strOrigin == "Center") return Origin::Center;

	return Origin::Init;
}

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

bool UILayout::IsEqual(const UILayout* other) const noexcept
{
	if (m_area != other->m_area) return false;
	if (m_origin != other->m_origin) return false;

	return true;
}

void UILayout::ToJson(ordered_json& outJson) const noexcept
{
	DataToJson("Area", m_area, outJson);
	DataToJson("Origin", m_origin, outJson);
}

void UILayout::FromJson(const json& j) noexcept
{
	DataFromJson("Area", m_area, j);
	DataFromJson("Origin", m_origin, j);
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

Vector2 UILayout::GetPosition(const Vector2& position) const noexcept
{
	return GetResolution() * position - m_originPoint;
}

bool UILayout::IsArea(const Vector2& pos) const noexcept
{
	return m_area.Contains(pos);
}

void UILayout::Union(const Rectangle& area) noexcept
{
	m_area = Rectangle::Union(m_area, area);
}