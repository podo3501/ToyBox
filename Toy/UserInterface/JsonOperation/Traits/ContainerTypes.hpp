#pragma once
#include "Traits.h"

template<typename K, typename T>
struct JsonTraits<unordered_map<K, T>>
{
	static nlohmann::ordered_json SerializeToJson(unordered_map<K, T>& datas)
	{
		nlohmann::ordered_json j;
		for (auto& [k, v] : datas)
			j[json_detail::ToKeyString(k)] = JsonTraits<T>::SerializeToJson(v);
		return j;
	}

	static unordered_map<K, T> DeserializeFromJson(const nlohmann::json& j)
	{
		unordered_map<K, T> datas;
		for (const auto& [k, v] : j.items())
			datas.emplace(json_detail::FromKeyString<K>(k), JsonTraits<T>::DeserializeFromJson(v));
		return datas;
	}
};