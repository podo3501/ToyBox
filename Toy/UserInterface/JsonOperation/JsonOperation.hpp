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

template <typename ProcessFunc>
static void ProcessWriteKey(const string& key, ProcessFunc processFunc, nlohmann::ordered_json& curWriteJ) noexcept
{
	nlohmann::ordered_json writeJ{};
	processFunc(writeJ);
	curWriteJ[key] = move(writeJ);
}

template <typename ProcessFunc>
static void ProcessReadKey(nlohmann::json& curReadJ, const string& key, ProcessFunc processFunc) noexcept
{
	if (!curReadJ.contains(key)) return;
	processFunc(curReadJ[key]);
}

template <typename WriteFunc, typename ReadFunc>
void JsonOperation::ProcessImpl(const string& key, WriteFunc&& writeFunc, ReadFunc&& readFunc) noexcept
{
	if (IsWrite()) 
		ProcessWriteKey(key, writeFunc, *m_wCurrent);
	else 
		ProcessReadKey(*m_rCurrent, key, readFunc);
}

template<HasSerializeIO T>
bool JsonOperation::WriteJsonToFile(T& obj, const wstring& filename)
{
	nlohmann::ordered_json writeJ;
	SerializeClassIO(obj, writeJ);
	JsonOperation js{ writeJ };
	if (!js.Write(filename)) return false;

	return true;
}

template<HasSerializeIO T>
bool JsonOperation::ReadJsonFromFile(const wstring& filename, T& obj)
{
	JsonOperation readJ;
	if (!readJ.Read(filename)) return false;
	DeserializeClassIO(readJ.GetRead(), obj);

	return true;
}