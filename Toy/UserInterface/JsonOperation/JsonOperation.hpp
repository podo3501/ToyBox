#pragma once
#include "JsonSerializationHelper.hpp"
#include  "JsonConcepts.h"

template<HasSerializeIO T>
bool WriteJsonToFile(T& obj, const wstring& filename)
{
	nlohmann::ordered_json writeJ;
	SerializeClassIO(obj, writeJ);
	JsonOperation js{ writeJ };
	if (!js.Write(filename)) return false;

	return true;
}

template<HasSerializeIO T>
bool ReadJsonFromFile(const wstring& filename, T& obj)
{
	JsonOperation readJ;
	if (!readJ.Read(filename)) return false;
	DeserializeClassIO(readJ.GetRead(), obj);

	return true;
}

//JsonOperation�� ����ؾ� �ϴ� Ÿ���� ���⼭ �б��� �ش�. �׷��� ������ JsonTraits���� �б�
template<typename T>
void JsonOperation::Process(const string& key, T& data) noexcept
{
	auto writeFunc = [this, &data](nlohmann::ordered_json& j) {
		if constexpr (HasSerializeIO<T>) SerializeClassIO(data, j); //SerializeIO �� �ִ� Ŭ����. unique_ptr�� �����ؼ�.
		else if constexpr (SequenceLike<T>) SerializeSeqContainer(data, j); //������ �����̳��϶� ó��
		else if constexpr (MapLike<T>) SerializeMapContainer(data, j); //Map �����̳��϶� ó��
		else j = JsonTraits<T>::SerializeToJson(data); }; //�Ϲ����� ������ �����϶� ó��

	auto readFunc = [this, &data](const nlohmann::json& j) {
		if constexpr (HasSerializeIO<T>) DeserializeClassIO(j, data);
		else if constexpr (SequenceLike<T>) DeserializeSeqContainer(j, data);
		else if constexpr (MapLike<T>) DeserializeMapContainer(j, data);
		else data = JsonTraits<T>::DeserializeFromJson(j); };

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