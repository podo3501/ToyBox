#pragma once
#include "JsonSerializationHelper.hpp"
#include  "JsonConcepts.h"

//JsonOperation을 사용해야 하는 타입은 여기서 분기해 준다. 그렇지 않으면 JsonTraits에서 분기
template<typename T>
void JsonOperation::Process(const string& key, T& data) noexcept
{
	auto writeFunc = [this, &data](nlohmann::ordered_json& j) {
		if constexpr (HasSerializeIO<T>) SerializeClassIO(data, j); //SerializeIO 가 있는 클래스. unique_ptr도 포함해서.
		else if constexpr (SequenceLike<T>) SerializeSeqContainer(data, j); //시퀀스 컨테이너일때 처리
		else if constexpr (MapLike<T>) SerializeMapContainer(data, j); //Map 컨테이너일때 처리
		else j = JsonTraits<T>::SerializeToJson(data); }; //일반적인 데이터 형식일때 처리

	auto readFunc = [this, &data](const nlohmann::json& j) {
		if constexpr (HasSerializeIO<T>) DeserializeClassIO(j, data);
		else if constexpr (SequenceLike<T>) DeserializeSeqContainer(j, data);
		else if constexpr (MapLike<T>) DeserializeMapContainer(j, data);
		else data = JsonTraits<T>::DeserializeFromJson(j); };

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