#pragma once
#include "JsonConcepts.h"

class UIComponent;
class UINameGenerator;

class JsonOperation
{
public:
	virtual ~JsonOperation();
	JsonOperation() noexcept;
	explicit JsonOperation(nlohmann::ordered_json& write) noexcept;
	explicit JsonOperation(const nlohmann::json& read) noexcept;	
	bool IsWrite();

	template<typename T>
	void Process(const string& key, T& data) noexcept;
	template<HasSerializeIO T>
	static bool WriteJsonToFile(T& obj, const wstring& filename);
	template<HasSerializeIO T>
	static bool ReadJsonFromFile(const wstring& filename, T& obj);

private:
	template <typename WriteFunc, typename ReadFunc>
	void ProcessImpl(const string& key, WriteFunc&& writeFunc, ReadFunc&& readFunc) noexcept;
	inline nlohmann::ordered_json& GetWrite() noexcept { return *m_wCurrent; }
	inline nlohmann::json& GetRead() const noexcept { return *m_rCurrent; }
	bool Write(const wstring& filename);
	bool Read(const wstring& filename);

	nlohmann::ordered_json m_write{};
	nlohmann::json m_read{};
	nlohmann::ordered_json* m_wCurrent{ nullptr }; //���鶧 �����ڿ� writeJ�� �־��ָ� m_write�� �Ⱦ��� ���������� writeJ�� ���ư���.
	nlohmann::json* m_rCurrent{ nullptr }; //���鶧  �����ڿ� readJ�� �־��ָ� m_read�� �Ⱦ��� ���������� readJ�� ���ư���.
};

#include "JsonOperation.hpp"

