#pragma once

template<typename T>
nlohmann::ordered_json JsonOperation::SerializeToJson(T& value)
{
	JsonOperation jsOp{};
	value.SerializeIO(jsOp);
	return jsOp.GetWrite();
}

template<>
nlohmann::ordered_json JsonOperation::SerializeToJson<XMUINT2>(nlohmann::ordered_json& j, XMUINT2& data) {
	j["x"] = data.x;
	j["y"] = data.y;
	return j;
}

template<typename T>
T JsonOperation::DeserializeFromJson(const nlohmann::json& v)
{
	T value{};
	JsonOperation jsOp{ v };
	value.SerializeIO(jsOp);
	return value;
}

template<>
void JsonOperation::DeserializeFromJson<XMUINT2>(const nlohmann::json& j, XMUINT2& data) {
    SafeRead(data.x, j["x"]);
    SafeRead(data.y, j["y"]);
}