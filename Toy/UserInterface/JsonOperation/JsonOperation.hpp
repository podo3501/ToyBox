#pragma once
#include "../UINameGenerator.h"
#include "Traits/JsonDetail.hpp"
#include "Traits/Traits.hpp"
//// Key ��ȯ ����
//template<typename K>
//struct KeyConverter {
//	static string ToKey(const K& key) { return key; }
//	static K FromKey(const string& key) { return key; }
//};
//
//// Ư��ȭ: wstring ó��
//template<>
//struct KeyConverter<wstring> {
//	static string ToKey(const wstring& key);
//	static wstring FromKey(const string& key);
//};
//
//// ��ȯ �Լ� ����
//template<typename K> string ToKeyString(const K& key) { return KeyConverter<K>::ToKey(key); }
//template<typename K> K FromKeyString(const string& key) { return KeyConverter<K>::FromKey(key); }

//Json�� �����ϴ� �⺻ Ÿ�� 
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

//�⺻ Ÿ���� �ƴ϶� Ŭ���� Ÿ���� ���
template<IsClass T>
void JsonOperation::Process(const string& key, T& data) noexcept
{
	if (IsWrite())
	{
		m_write->GotoKey(key);
		data.SerializeIO(*this);
		m_write->GoBack();
	}
	else
	{
		m_read->GotoKey(key);
		data.SerializeIO(*this);
		m_read->GoBack();
	}
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

//UIComponent overload �� �Լ��� �����Ѵ�. �׷��� �̰� Component�� �ƴ� unique_ptr�� ���� �Լ�
template<IsNotUIComponent T>
void JsonOperation::Process(const string& key, unique_ptr<T>& data)
{
	if (IsWrite())
	{
		m_write->GotoKey(key);
		data->SerializeIO(*this);
		m_write->GoBack();
	}
	else
	{
		m_read->GotoKey(key);
		data = CreateData<T>();
		m_read->GoBack();
	}
}

//�迭 ������ ������ �ֱ� ������ push_back���� ���� �־�� �Ѵ�.
template <typename ProcessFunc>
void JsonOperation::ProcessWriteKey(const string& key, ProcessFunc processFunc)
{
	m_write->GotoKey(key, true);
	processFunc(m_write->GetCurrent());
	m_write->GoBack();
}

template <typename ProcessFunc>
void JsonOperation::ProcessReadKey(const string& key, ProcessFunc processFunc)
{
	const auto& j = m_read->GetCurrent();
	if (!j.contains(key))
		return;

	m_read->GotoKey(key);
	processFunc(m_read->GetCurrent());
	m_read->GoBack();
}

template <typename WriteFunc, typename ReadFunc>
void JsonOperation::ProcessImpl(const string& key, WriteFunc&& writeFunc, ReadFunc&& readFunc)
{
	if (IsWrite())
	{
		m_write->GotoKey(key);
		writeFunc(m_write->GetCurrent());
		m_write->GoBack();
	}
	else
	{
		m_read->GotoKey(key);
		readFunc(m_read->GetCurrent());
		m_read->GoBack();
	}
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
nlohmann::ordered_json JsonOperation::SerializeMapContainer(Container& datas)
{
	nlohmann::ordered_json j;
	using K = typename Container::key_type;
	using T = typename Container::mapped_type;

	for (auto& [k, v] : datas) 
	{
		if constexpr (HasSerializeIO<T>) 
		{
			JsonOperation jsOp{};
			v.SerializeIO(jsOp);
			j[json_detail::ToKeyString(k)] = jsOp.GetWrite();
		}
		else
			j[json_detail::ToKeyString(k)] = JsonTraits<T>::SerializeToJson(v);
	}
	return j;
}

template<typename Container>
void JsonOperation::DeserializeMapContainer(Container& datas, const nlohmann::ordered_json& j) 
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
	//nlohmann::ordered_json j;
	//auto writeFunc = [&datas](auto& j) {
	//	for (auto& [k, v] : datas) {
	//		if constexpr (HasSerializeIO<T>) {
	//			JsonOperation jsOp{};
	//			v.SerializeIO(jsOp);
	//			j[json_detail::ToKeyString(k)] = jsOp.GetWrite();
	//		}
	//		else
	//			j[json_detail::ToKeyString(k)] = JsonTraits<T>::SerializeToJson(v);
	//	}
	//	return j;
	//	};

	//auto readFunc = [&datas](const auto& j) {
	//	for (const auto& [k, v] : j.items()) {
	//		if constexpr (HasSerializeIO<T>) {
	//			T data{};
	//			JsonOperation jsOp{ v };
	//			data.SerializeIO(jsOp);
	//			datas.emplace(json_detail::FromKeyString<K>(k), data);
	//		}
	//		else
	//			datas = JsonTraits<unordered_map<K, T>>::DeserializeFromJson(v);
	//	}};

	//ProcessImpl(key, writeFunc, readFunc);

	auto writeFunc = [this, &datas](auto&) { return SerializeMapContainer(datas); };
	auto readFunc = [this, &datas](const auto& j) { DeserializeMapContainer(datas, j); };

	ProcessImpl(key, writeFunc, readFunc);
}