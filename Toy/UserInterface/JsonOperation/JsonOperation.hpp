#pragma once
#include "../UINameGenerator.h"
#include "JsonSerializationHelper.hpp"

//Json이 지원하는 기본 타입 
template<Available T>
void JsonOperation::Process(const string& key, T& data) noexcept
{
	auto writeFunc = [this, &data](nlohmann::ordered_json& j) { j = data; };
	auto readFunc = [this, &data](const nlohmann::json& j) { data = j; };

	ProcessImpl(key, writeFunc, readFunc);
}

//기본 타입이 아니라 클래스 타입일 경우
template<IsClass T>
void JsonOperation::Process(const string& key, T& data) noexcept
{
	auto writeFunc = [this, &data](nlohmann::ordered_json& j) { j = SerializeClassIO(data); };
	auto readFunc = [this, &data](const nlohmann::json& j) { data = DeserializeClassIO<T>(j); };

	ProcessImpl(key, writeFunc, readFunc);
}

//UIComponent overload 된 함수가 존재한다. 그래서 이건 Component가 아닌 unique_ptr에 관한 함수
template<IsNotUIComponent T>
void JsonOperation::Process(const string& key, unique_ptr<T>& data)
{
	auto writeFunc = [this, &data](nlohmann::ordered_json& j) { j = SerializeClassIO(*data); };
	auto readFunc = [this, &data](const nlohmann::json& j) { DeserializeClassIO(j, data); };

	ProcessImpl(key, writeFunc, readFunc);
}

//배열 형식을 가지고 있기 때문에 push_back으로 값을 넣어야 한다.
template <typename ProcessFunc>
void JsonOperation::ProcessWriteKey(const string& key, ProcessFunc processFunc)
{
	nlohmann::ordered_json writeJ{};
	processFunc(writeJ);
	nlohmann::ordered_json& currentJ = GetWrite();
	currentJ[key] = move(writeJ);
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
	auto writeFunc = [this, &datas](nlohmann::ordered_json& j) { SerializeSeqContainer(datas, j); };
	auto readFunc = [this, &datas](const nlohmann::json& j) { DeserializeSeqContainer(j, datas); };

	ProcessImpl(key, writeFunc, readFunc);
}

template<typename Container>
void JsonOperation::Process(const string& key, Container& datas) noexcept
{
	auto writeFunc = [this, &datas](nlohmann::ordered_json& j) { SerializeMapContainer(datas, j); };
	auto readFunc = [this, &datas](const nlohmann::json& j) { DeserializeMapContainer(j, datas); };

	ProcessImpl(key, writeFunc, readFunc);
}