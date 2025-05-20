#pragma once
#include "../JsonOperation.h"

template<typename T>
struct JsonTraits
{
	static nlohmann::ordered_json SerializeToJson(T& data)
	{
		JsonOperation jsOp{};
		data.SerializeIO(jsOp);
		return jsOp.GetWrite();
	}

	static T DeserializeFromJson(const nlohmann::json& dataJ)
	{
		T data{};
		JsonOperation jsOp{ dataJ };
		data.SerializeIO(jsOp);
		return data;
	}
};
