#pragma once
#include "JsonSerializer.hpp"
#include  "JsonConcepts.h"

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