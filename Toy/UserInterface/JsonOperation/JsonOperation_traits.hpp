#pragma once

template<typename T, typename J>
static void SafeRead(T& out, const J& value)
{
	Assert(!value.is_null());
	out = value.get<T>();
}

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

template<>
struct JsonTraits<XMINT2>
{
	static nlohmann::ordered_json SerializeToJson(XMINT2& data)
	{
		nlohmann::ordered_json j;
        j["x"] = data.x;
        j["y"] = data.y;
		return j;
	}

	static XMINT2 DeserializeFromJson(const nlohmann::json& dataJ)
	{
		XMINT2 data{};
		SafeRead(data.x, dataJ["x"]);
		SafeRead(data.y, dataJ["y"]);
		return data;
	}
};

template<>
struct JsonTraits<XMUINT2>
{
	static nlohmann::ordered_json SerializeToJson(XMUINT2& data) 
	{
		nlohmann::ordered_json j;
		j["x"] = data.x;
		j["y"] = data.y;
		return j;
	}

	static XMUINT2 DeserializeFromJson(const nlohmann::json& dataJ)
	{
		XMUINT2 data{};
		SafeRead(data.x, dataJ["x"]);
		SafeRead(data.y, dataJ["y"]);
		return data;
	}
};
