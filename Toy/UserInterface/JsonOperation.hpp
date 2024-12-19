#pragma once
#include "JsonOperation.h"

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
