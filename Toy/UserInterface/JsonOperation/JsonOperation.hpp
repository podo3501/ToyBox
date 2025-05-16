#pragma once
#include "../UINameGenerator.h"

// Key ��ȯ ����
template<typename K>
struct KeyConverter {
	static string ToKey(const K& key) { return key; }
	static K FromKey(const string& key) { return key; }
};

// Ư��ȭ: wstring ó��
template<>
struct KeyConverter<wstring> {
	static string ToKey(const wstring& key);
	static wstring FromKey(const string& key);
};

// ��ȯ �Լ� ����
template<typename K> string ToKeyString(const K& key) { return KeyConverter<K>::ToKey(key); }
template<typename K> K FromKeyString(const string& key) { return KeyConverter<K>::FromKey(key); }

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

template<typename K, typename T>
void JsonOperation::Process(const string& key, unordered_map<K, T>& datas) noexcept
{
	auto writeFunc = [this, &datas](auto& j) {
		for (auto& [k, v] : datas)
			j[ToKeyString(k)] = SerializeToJson(v);
		};

	auto readFunc = [this, &datas](const auto& j) {
		datas.clear();
		for (auto& [k, v] : j.items())
			datas.emplace(FromKeyString<K>(k), DeserializeFromJson<T>(v));
		};

	ProcessImpl(key, writeFunc, readFunc);
}