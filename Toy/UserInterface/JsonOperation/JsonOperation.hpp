#pragma once
#include "JsonOperation.h"
#include "../UINameGenerator.h"

//map, unordered_map키 값 반환하는 함수
template<typename Key>
struct KeyConverter;
template<> struct KeyConverter<int> { static string ToKey(int key) { return to_string(key); } };
template<> struct KeyConverter<wstring> { static string ToKey(const wstring& key); };

template<typename Key>
std::string ToKeyString(const Key& key) {
	return KeyConverter<Key>::ToKey(key);
}

template<typename K, typename = void>
struct FromKeyStringHelper;

template<> struct FromKeyStringHelper<int> { static int Convert(const string& key) { return stoi(key); } };
template<> struct FromKeyStringHelper<wstring> { static wstring Convert(const string& key); };

template<typename K>
K FromKeyString(const string& key) {
	return FromKeyStringHelper<K>::Convert(key);
}

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

//UIComponent overload 된 함수가 존재한다. 그래서 이건 Component가 아닌 unique_ptr에 관한 함수
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

//배열 형식을 가지고 있기 때문에 push_back으로 값을 넣어야 한다.
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

template<typename T>
nlohmann::ordered_json JsonOperation::SerializeToJson(T& value)
{
	JsonOperation jsOp{};
	value.SerializeIO(jsOp);
	return jsOp.GetWrite();
}

template<typename T>
T JsonOperation::DeserializeFromJson(const nlohmann::json& v)
{
	T value{};
	JsonOperation jsOp{ v };
	value.SerializeIO(jsOp);
	return value;
}

template<typename T>
void JsonOperation::Process(const string& key, unordered_map<string, T>& datas) noexcept
{
	auto writeFunc = [this, &datas](auto& j) {
		for (auto& [k, v] : datas)
			j[k] = SerializeToJson(v);
		};

	auto readFunc = [this, &datas](const auto& j) {
		datas.clear();
		for (auto& [k, v] : j.items())
			datas.emplace(k, DeserializeFromJson<T>(v));
		};

	ProcessImpl(key, writeFunc, readFunc);
}