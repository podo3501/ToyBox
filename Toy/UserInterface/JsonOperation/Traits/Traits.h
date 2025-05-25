#pragma once
#include "JsonDetail.hpp"

template<typename T>
struct JsonTraitsBase
{
	static nlohmann::ordered_json SerializeToJson(const T& data);
	static T DeserializeFromJson(const nlohmann::json& dataJ);
};

template<typename T>
struct JsonTraits : public JsonTraitsBase<T> {};
