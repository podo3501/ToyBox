#pragma once
#include "../UINameGenerator.h"
#include "JsonSerializationHelper.hpp"

//Json�� �����ϴ� �⺻ Ÿ�� 
template<Available T>
void JsonOperation::Process(const string& key, T& data) noexcept
{
	auto writeFunc = [this, &data](nlohmann::ordered_json& j) { j = data; };
	auto readFunc = [this, &data](const nlohmann::json& j) { data = j; };

	ProcessImpl(key, writeFunc, readFunc);
}

//�⺻ Ÿ���� �ƴ϶� Ŭ���� Ÿ���� ���
template<IsClass T>
void JsonOperation::Process(const string& key, T& data) noexcept
{
	auto writeFunc = [this, &data](nlohmann::ordered_json& j) { j = SerializeClassIO(data); };
	auto readFunc = [this, &data](const nlohmann::json& j) { data = DeserializeClassIO<T>(j); };

	ProcessImpl(key, writeFunc, readFunc);
}

//UIComponent overload �� �Լ��� �����Ѵ�. �׷��� �̰� Component�� �ƴ� unique_ptr�� ���� �Լ�
template<IsNotUIComponent T>
void JsonOperation::Process(const string& key, unique_ptr<T>& data)
{
	auto writeFunc = [this, &data](nlohmann::ordered_json& j) { j = SerializeClassIO(*data); };
	auto readFunc = [this, &data](const nlohmann::json& j) { DeserializeClassIO(j, data); };

	ProcessImpl(key, writeFunc, readFunc);
}

//�迭 ������ ������ �ֱ� ������ push_back���� ���� �־�� �Ѵ�.
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