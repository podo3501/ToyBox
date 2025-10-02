#include "pch.h"
#include "Traits.h"
#include "../../UIComponent/UIType.h"
#include "Shared/Utils/StringExt.h"

nlohmann::ordered_json JsonTraitsBase<XMINT2>::SerializeToJson(const XMINT2& data) { return json_detail::SerializeXY(data); }
XMINT2 JsonTraitsBase<XMINT2>::DeserializeFromJson(const nlohmann::json& dataJ)
{
	XMINT2 data{};
	json_detail::DeserializeXY(data, dataJ);
	return data;
}

/////////////////////////////////////////////////////////////////

nlohmann::ordered_json JsonTraitsBase<XMUINT2>::SerializeToJson(const XMUINT2& data) { return json_detail::SerializeXY(data); }
XMUINT2 JsonTraitsBase<XMUINT2>::DeserializeFromJson(const nlohmann::json& dataJ)
{
	return json_detail::CreateAndFill<XMUINT2>([&dataJ](XMUINT2& data) {
		json_detail::DeserializeXY(data, dataJ); });
}

/////////////////////////////////////////////////////////////////

nlohmann::ordered_json JsonTraitsBase<Rectangle>::SerializeToJson(const Rectangle& data)
{
	auto j = json_detail::SerializeXY(data);
	return json_detail::SerializeRectExtra(data, j);
}

Rectangle JsonTraitsBase<Rectangle>::DeserializeFromJson(const nlohmann::json& dataJ)
{
	return json_detail::CreateAndFill<Rectangle>([&dataJ](Rectangle& data) {
		json_detail::DeserializeXY(data, dataJ);
		json_detail::DeserializeRectExtra(data, dataJ); });
}

/////////////////////////////////////////////////////////////////

nlohmann::ordered_json JsonTraitsBase<Origin>::SerializeToJson(const Origin& data) { return EnumToString(data); }
Origin JsonTraitsBase<Origin>::DeserializeFromJson(const nlohmann::json& dataJ)
{
	return json_detail::CreateAndFill<Origin>([&dataJ](Origin& data) {
		data = *StringToEnum<Origin>(dataJ); });
}

/////////////////////////////////////////////////////////////////

static double RoundToSixA(double value) noexcept { return round(value * 1e6) / 1e6; }
nlohmann::ordered_json JsonTraitsBase<Vector2>::SerializeToJson(const Vector2& data)
{
	Assert(!isnan(data.x));
	Assert(!isnan(data.y));
	nlohmann::ordered_json j;
	j["x"] = RoundToSixA(data.x);
	j["y"] = RoundToSixA(data.y);
	return j;
}

Vector2 JsonTraitsBase<Vector2>::DeserializeFromJson(const nlohmann::json& dataJ)
{
	return json_detail::CreateAndFill<Vector2>([&dataJ](Vector2& data) {
		json_detail::DeserializeXY(data, dataJ); });
}

/////////////////////////////////////////////////////////////////

nlohmann::ordered_json JsonTraitsBase<string>::SerializeToJson(const string& data) { return data; }
string JsonTraitsBase<string>::DeserializeFromJson(const nlohmann::json& dataJ) { return dataJ; }

/////////////////////////////////////////////////////////////////

nlohmann::ordered_json JsonTraitsBase<wstring>::SerializeToJson(const wstring& data) { return WStringToString(data); }
wstring JsonTraitsBase<wstring>::DeserializeFromJson(const nlohmann::json& dataJ)
{
	return json_detail::CreateAndFill<wstring>([&dataJ](wstring& data) {
		data = StringToWString(dataJ); });
}
