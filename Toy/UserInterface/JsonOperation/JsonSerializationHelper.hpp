#pragma once
#include "Traits/Traits.h"
#include "JsonConcepts.h"

class UIComponent;

template<typename T>
void SerializeClassIO_Internal(T& data, nlohmann::ordered_json& j)
{
	JsonOperation jsOp{};
	data.SerializeIO(jsOp);
	j = jsOp.GetWrite();
}

template<typename T>
void SerializeClassIO(T& data, nlohmann::ordered_json& j)
{
	SerializeClassIO_Internal(data, j);
}

template<typename T>
void SerializeClassIO(unique_ptr<T>& data, nlohmann::ordered_json& j)
{
	SerializeClassIO_Internal(*data, j);
}

void SerializeClassIO(unique_ptr<UIComponent>& data, nlohmann::ordered_json& j);

///////////////////////////////////////////////////////

template<typename T>
void DeserializeClassIO_Internal(const nlohmann::json& j, T& data)
{
	JsonOperation jsOp{ j };
	data.SerializeIO(jsOp);
}

template<typename T>
void DeserializeClassIO(const nlohmann::json& j, T& data)
{
	DeserializeClassIO_Internal(j, data);
}

template<typename T>
void DeserializeClassIO(const nlohmann::json& j, unique_ptr<T>& data)
{
	DeserializeClassIO_Internal(j, *data);
}

void DeserializeClassIO(const nlohmann::json& j, unique_ptr<UIComponent>& data);

template<typename T>
nlohmann::ordered_json SerializeByType(T& v)
{
	if constexpr (HasSerializeIO<T>)
	{
		nlohmann::ordered_json j{};
		SerializeClassIO(v, j);
		return j;
	}
	else
		return JsonTraits<T>::SerializeToJson(v);
}

template<typename T>
T DeserializeByType(const nlohmann::json& v)
{
	if constexpr (HasSerializeIO<T>)
	{
		T data{};
		DeserializeClassIO(v, data);
		return data;
	}
	else
		return JsonTraits<T>::DeserializeFromJson(v);
}

template<typename MapContainer>
void SerializeMapContainer(MapContainer& datas, nlohmann::ordered_json& j)
{
	using T = typename MapContainer::mapped_type;

	for (auto& [k, v] : datas)
	{
		const auto key = json_detail::ToKeyString(k);
		j.emplace(key, SerializeByType<T>(v));
	}
}

template<typename MapContainer>
void DeserializeMapContainer(const nlohmann::json& j, MapContainer& datas)
{
	using K = typename MapContainer::key_type;
	using T = typename MapContainer::mapped_type;

	for (const auto& [k, v] : j.items())
	{
		const auto key = json_detail::FromKeyString<K>(k);
		datas.emplace(key, DeserializeByType<T>(v));
	}
}

template<typename SeqContainer>
void SerializeSeqContainer(SeqContainer& datas, nlohmann::ordered_json& j)
{
	for (auto& data : datas)
		j.push_back(SerializeByType(data));
}

template<typename SeqContainer>
void DeserializeSeqContainer(const nlohmann::json& j, SeqContainer& datas)
{
	for (const auto& dataJ : j)
	{
		using T = typename SeqContainer::value_type;
		datas.emplace_back(DeserializeByType<T>(dataJ));
	}
}