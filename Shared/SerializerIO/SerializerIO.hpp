#pragma once
#include "ClassSerializeIO.hpp"
#include  "Concepts.h"

//JsonOperation�� ����ؾ� �ϴ� Ÿ���� ���⼭ �б��� �ش�. �׷��� ������ JsonTraits���� �б�
template<typename T>
void SerializerIO::Process(const string& key, T& data) noexcept
{
	auto writeFunc = [this, &data](nlohmann::ordered_json& j) {
		if constexpr (HasProcessIO<T>) SerializeClassIO(data, j); //SerializeIO �� �ִ� Ŭ����. unique_ptr�� �����ؼ�.
		else if constexpr (SequenceLike<T>) SerializeSeqContainer(data, j); //������ �����̳��϶� ó��
		else if constexpr (MapLike<T>) SerializeMapContainer(data, j); //Map �����̳��϶� ó��
		else j = JsonTraits<T>::SerializeToJson(data); }; //�Ϲ����� ������ �����϶� ó��

	auto readFunc = [this, &data](const nlohmann::json& j) {
		if constexpr (HasProcessIO<T>) DeserializeClassIO(j, data);
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
void SerializerIO::ProcessImpl(const string& key, WriteFunc&& writeFunc, ReadFunc&& readFunc) noexcept
{
	if (IsWrite()) 
		ProcessWriteKey(key, writeFunc, *m_wCurrent);
	else 
		ProcessReadKey(*m_rCurrent, key, readFunc);
}

template<HasProcessIO T>
bool SerializerIO::WriteJsonToFile(T& obj, const wstring& filename)
{
	nlohmann::ordered_json writeJ;
	SerializeClassIO(obj, writeJ);
	SerializerIO serializer{ writeJ };
	if (!serializer.Write(filename)) return false;

	return true;
}

template<HasProcessIO T>
bool SerializerIO::ReadJsonFromFile(const wstring& filename, T& obj)
{
	SerializerIO serializer;
	if (!serializer.Read(filename)) return false;
	DeserializeClassIO(serializer.GetRead(), obj);

	return true;
}