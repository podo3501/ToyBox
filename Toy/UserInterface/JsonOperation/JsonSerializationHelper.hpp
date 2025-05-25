#pragma once
#include "Traits/Traits.h"

template<typename T>
nlohmann::ordered_json SerializeClassIO(T& data)
{
	JsonOperation jsOp{};
	data.SerializeIO(jsOp);
	return jsOp.GetWrite();
}

template<typename T>
void DeserializeClassIO_Internal(const nlohmann::json& j, T& data)
{
	JsonOperation jsOp{ j };
	data.SerializeIO(jsOp);
}

template<typename T>
T DeserializeClassIO(const nlohmann::json& j)
{
	T data{};
	DeserializeClassIO_Internal(j, data);
	return data;
}

template<typename T>
void DeserializeClassIO(const nlohmann::json& j, unique_ptr<T>& data)
{
	DeserializeClassIO_Internal(j, *data);
}

template<typename T>
nlohmann::ordered_json SerializeByType(T& v)
{
	if constexpr (HasSerializeIO<T>)
		return SerializeClassIO(v);
	else
		return JsonTraits<T>::SerializeToJson(v);
}

template<typename Container>
void SerializeMapContainer(Container& datas, nlohmann::ordered_json& j)
{
	using T = typename Container::mapped_type;

	for (auto& [k, v] : datas)
	{
		const auto key = json_detail::ToKeyString(k);
		j.emplace(key, move(SerializeByType<T>(v)));
	}
}

template<typename T>
T DeserializeByType(const nlohmann::json& v)
{
	if constexpr (HasSerializeIO<T>)
		return DeserializeClassIO<T>(v);
	else
		return JsonTraits<T>::DeserializeFromJson(v);
}

template<typename Container>
void DeserializeMapContainer(const nlohmann::json& j, Container& datas)
{
	using K = typename Container::key_type;
	using T = typename Container::mapped_type;

	for (const auto& [k, v] : j.items())
	{
		const auto key = json_detail::FromKeyString<K>(k);
		datas.emplace(move(key), move(DeserializeByType<T>(v)));
	}
}