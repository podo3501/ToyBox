#pragma once
#include "../UINameGenerator.h"
#include "Traits/Traits.h"

//Json이 지원하는 기본 타입 
template<Available T>
void JsonOperation::Process(const string& key, T& data) noexcept
{
	if (IsWrite())
	{
		auto& j = GetWrite();
		j[key] = data;
	}
	else
	{
		auto& j = GetRead();
		data = j[key];
	}
}

//기본 타입이 아니라 클래스 타입일 경우
template<IsClass T>
void JsonOperation::Process(const string& key, T& data) noexcept
{
	auto writeFunc = [this, &data](nlohmann::ordered_json& j) {
		JsonOperation jsOp{};
		data.SerializeIO(jsOp);
		j = jsOp.GetWrite(); };

	auto readFunc = [this, &data](const nlohmann::json& j) { 
		JsonOperation jsOp{ j };
		data.SerializeIO(jsOp); };

	ProcessImpl(key, writeFunc, readFunc);
}

//UIComponent overload 된 함수가 존재한다. 그래서 이건 Component가 아닌 unique_ptr에 관한 함수
template<IsNotUIComponent T>
void JsonOperation::Process(const string& key, unique_ptr<T>& data)
{
	auto writeFunc = [this, &data](nlohmann::ordered_json& j) {
		JsonOperation jsOp{};
		data->SerializeIO(jsOp);
		j = jsOp.GetWrite(); };

	auto readFunc = [this, &data](const nlohmann::json& j) {
		JsonOperation jsOp{ j };
		data->SerializeIO(jsOp); };

	ProcessImpl(key, writeFunc, readFunc);
}

template<typename T>
unique_ptr<T> JsonOperation::CreateData(const nlohmann::json& readJ)
{
	JsonOperation js(readJ);
	auto comp = std::make_unique<T>();
	if (readJ == nullptr)
		comp->SerializeIO(*this);
	else
		comp->SerializeIO(js);
	return comp;
}

//배열 형식을 가지고 있기 때문에 push_back으로 값을 넣어야 한다.
template <typename ProcessFunc>
void JsonOperation::ProcessWriteKey(const string& key, ProcessFunc processFunc)
{
	nlohmann::ordered_json writeJ{};
	processFunc(writeJ);
	GetWrite()[key] = writeJ;
}

template <typename ProcessFunc>
void JsonOperation::ProcessReadKey(const string& key, ProcessFunc processFunc)
{
	const auto& readJ = GetRead();
	if (!readJ.contains(key))
		return;

	processFunc(readJ[key]);
}

template <typename WriteFunc, typename ReadFunc>
void JsonOperation::ProcessImpl(const string& key, WriteFunc&& writeFunc, ReadFunc&& readFunc)
{
	if (IsWrite()) 
		ProcessWriteKey(key, writeFunc);
	else 
		ProcessReadKey(key, readFunc);
}

template<IsClassContainer T>
void JsonOperation::Process(const string& key, T& datas) noexcept
{
	if (IsWrite())
	{
		if (datas.empty())
			return;

		ProcessWriteKey(key, [&datas](auto& currentJson) {
			for (auto& data : datas)
			{
				JsonOperation jsOp{};
				data.SerializeIO(jsOp);
				currentJson.push_back(jsOp.GetWrite());
			}
			});
	}
	else
	{
		datas.clear();
		ProcessReadKey(key, [&datas, this](const auto& currentJson) {
			for (const auto& compJson : currentJson)
			{
				typename T::value_type data{};
				JsonOperation jsOp(compJson);
				data.SerializeIO(jsOp);
				datas.emplace_back(move(data));
			}
			});
	}
}

template<typename T>
void JsonOperation::Process(const string& key, vector<unique_ptr<T>>& data)
{
	if (IsWrite())
	{
		if (data.empty())
			return;

		ProcessWriteKey(key, [&data](auto& currentJson) {
			for (auto& comp : data)
			{
				JsonOperation jsOp{};
				comp->SerializeIO(jsOp);
				currentJson.push_back(jsOp.GetWrite());
			}
			});
	}
	else
	{
		data.clear();
		ProcessReadKey(key, [&data, this](const auto& currentJson) {
			for (const auto& compJson : currentJson)
				data.emplace_back(CreateData<T>(compJson));
			});
	}
}

template<typename T>
void JsonOperation::Process(const string& key, vector<T>& datas)
{
	if (IsWrite())
	{
		if (datas.empty())
			return;

		ProcessWriteKey(key, [&datas, idx{ 0 }](auto& currentJson) mutable {
			for (auto& data : datas)
			{
				JsonOperation jsOp{};
				jsOp.Process(to_string(idx++), data);
				currentJson.push_back(jsOp.GetWrite());
			}
			});
	}
	else
	{
		datas.clear();
		ProcessReadKey(key, [&datas, idx{ 0 }](const auto& dataJsons) mutable {
			for (const auto& dataJson : dataJsons)
			{
				T tempValue;
				JsonOperation js(dataJson);
				js.Process(to_string(idx++), tempValue);
				datas.emplace_back(tempValue);
			}
			});
	}
}

template<typename T>
void JsonOperation::Process(const string& key, Property<T>& data)
{
	if (IsWrite())
	{
		T tempValue = data.Get();
		Process(key, tempValue);
	}
	else
	{
		T tempValue{};
		Process(key, tempValue);
		data.Set(tempValue);
	}
}

template<typename Container>
void JsonOperation::SerializeMapContainer(const Container& datas, nlohmann::ordered_json& j)
{
	using T = typename Container::mapped_type;

	for (const auto& [k, v] : datas) 
	{
		if constexpr (HasSerializeIO<T>) 
		{
			JsonOperation jsOp{};
			const_cast<T&>(v).SerializeIO(jsOp);  //const를 제거한다. SerializeIO 함수는 입출력을 둘다 담당하기 때문에 const가 될수 없다.
			j[json_detail::ToKeyString(k)] = jsOp.GetWrite();
		}
		else
			j[json_detail::ToKeyString(k)] = JsonTraits<T>::SerializeToJson(v);
	}
}

template<typename Container>
void JsonOperation::DeserializeMapContainer(const nlohmann::ordered_json& j, Container& datas)
{
	using K = typename Container::key_type;
	using T = typename Container::mapped_type;

	for (const auto& [k, v] : j.items()) 
	{
		if constexpr (HasSerializeIO<T>) 
		{
			T data{};
			JsonOperation jsOp{ v };
			data.SerializeIO(jsOp);
			datas.emplace(json_detail::FromKeyString<K>(k), move(data));
		}
		else
			datas = JsonTraits<Container>::DeserializeFromJson(v);
	}
}

template<typename K, typename T>
void JsonOperation::Process(const string& key, unordered_map<K, T>& datas) noexcept
{
	auto writeFunc = [this, &datas](nlohmann::ordered_json& j) { SerializeMapContainer(datas, j); };
	auto readFunc = [this, &datas](const nlohmann::json& j) { DeserializeMapContainer(j, datas); };

	ProcessImpl(key, writeFunc, readFunc);
}