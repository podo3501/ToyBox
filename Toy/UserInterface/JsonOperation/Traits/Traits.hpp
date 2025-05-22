#pragma once

template<typename T>
struct JsonTraitsBase
{
	static nlohmann::ordered_json SerializeToJson(T& data);
	//{
	//	data;
	//	//JsonOperation jsOp{};
	//	//data.SerializeIO(jsOp);
	//	//return jsOp.GetWrite();
	//	Assert(false);
	//	return {};
	//}

	static T DeserializeFromJson(const nlohmann::json& dataJ);
	//{
	//	dataJ;
	//	//T data{};
	//	//JsonOperation jsOp{ dataJ };
	//	//data.SerializeIO(jsOp);
	//	//return data;
	//	Assert(false);
	//	return {};
	//} 
};

template<typename T>
struct JsonTraits : public JsonTraitsBase<T> {};
