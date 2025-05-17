#pragma once

template<typename T>
struct JsonTraits
{
	static nlohmann::ordered_json SerializeToJson(T& value)
	{
		JsonOperation jsOp{};
		value.SerializeIO(jsOp);
		return jsOp.GetWrite();
	}

	static T DeserializeFromJson(const nlohmann::json& v)
	{
		T value{};
		JsonOperation jsOp{ v };
		value.SerializeIO(jsOp);
		return value;
	}
};

//template<>
//nlohmann::ordered_json JsonOperation::SerializeToJson<XMUINT2>(XMUINT2& data) {
//	JsonOperation jsOp{};
//	jsOp["x"] = data.x;
//	jsOp["y"] = data.y;
//	return jsOp.GetWrite();
//}

//template<>
//void JsonOperation::DeserializeFromJson<XMUINT2>(const nlohmann::json& j, XMUINT2& data) {
//    SafeRead(data.x, j["x"]);
//    SafeRead(data.y, j["y"]);
//}
