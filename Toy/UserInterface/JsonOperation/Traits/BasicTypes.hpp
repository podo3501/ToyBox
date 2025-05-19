#pragma once
#include "Traits.hpp"
#include "JsonDetail.hpp"
#include "../../UIComponent/UIType.h"
#include "Utility.h"

template<>
struct JsonTraits<XMINT2>
{
	static nlohmann::ordered_json SerializeToJson(XMINT2& data) { return json_detail::SerializeXY(data); }
	static XMINT2 DeserializeFromJson(const nlohmann::json& dataJ)
	{
		XMINT2 data{};
		json_detail::DeserializeXY(data, dataJ);
		return data;
	}
};

template<>
struct JsonTraits<XMUINT2>
{
	static nlohmann::ordered_json SerializeToJson(XMUINT2& data) { return json_detail::SerializeXY(data); }
	static XMUINT2 DeserializeFromJson(const nlohmann::json& dataJ)
	{
		return json_detail::CreateAndFill<XMUINT2>([&dataJ](XMUINT2& data) {
			json_detail::DeserializeXY(data, dataJ); });
	}
};

template<>
struct JsonTraits<Rectangle>
{
	static nlohmann::ordered_json SerializeToJson(Rectangle& data)
	{
		auto j = json_detail::SerializeXY(data);
		return json_detail::SerializeRectExtra(data, j);
	}

	static Rectangle DeserializeFromJson(const nlohmann::json& dataJ)
	{
		return json_detail::CreateAndFill<Rectangle>([&dataJ](Rectangle& data) {
			json_detail::DeserializeXY(data, dataJ);
			json_detail::DeserializeRectExtra(data, dataJ); });
	}
};

template<>
struct JsonTraits<Origin>
{
	static nlohmann::ordered_json SerializeToJson(Origin& data) { return EnumToString(data); }
	static Origin DeserializeFromJson(const nlohmann::json& dataJ)
	{
		return json_detail::CreateAndFill<Origin>([&dataJ](Origin& data) {
			data = *StringToEnum<Origin>(dataJ); });
	}
};

template<>
struct JsonTraits<Vector2>
{
	static double RoundToSixA(double value) noexcept { return round(value * 1e6) / 1e6; }
	static nlohmann::ordered_json SerializeToJson(Vector2& data) 
	{
		Assert(!isnan(data.x));
		Assert(!isnan(data.y));
		nlohmann::ordered_json j;
		j["x"] = RoundToSixA(data.x);
		j["y"] = RoundToSixA(data.y);
		return j;
	}
	static Vector2 DeserializeFromJson(const nlohmann::json& dataJ)
	{
		return json_detail::CreateAndFill<Vector2>([&dataJ](Vector2& data) {
			json_detail::DeserializeXY(data, dataJ); });
	}
};

template<>
struct JsonTraits<wstring>
{
	static nlohmann::ordered_json SerializeToJson(wstring& data) { return WStringToString(data); }
	static wstring DeserializeFromJson(const nlohmann::json& dataJ)
	{
		return json_detail::CreateAndFill<wstring>([&dataJ](wstring& data) {
			data = StringToWString(dataJ); });
	}
};