#include "pch.h"
#include "UILayout.h"
#include "UIType.h"
#include "../Config.h"
#include "JsonHelper.h"
#include "JsonOperation.h"

using json = nlohmann::json;
using ordered_json = nlohmann::ordered_json;

UILayout::~UILayout() = default;
Origin GetOrigin(const string& strOrigin)
{
	if (strOrigin == "LeftTop") return Origin::LeftTop;
	if (strOrigin == "Center") return Origin::Center;

	return Origin::Init;
}

UILayout& UILayout::operator=(const UILayout& other) = default;

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

void to_json(nlohmann::ordered_json& j, const UILayout& data)
{
	DataToJson("Area", data.m_area, j);
	DataToJson("Origin", data.m_origin, j);
}

void from_json(const nlohmann::json& j, UILayout& data)
{
	DataFromJson("Area", data.m_area, j);
	DataFromJson("Origin", data.m_origin, j);
}

void UILayout::SerializeIO(JsonOperation* operation)
{
	operation->Process("Area", m_area);
	operation->Process("Origin", m_origin);
}