#pragma once

class UIComponent;
class UINameGenerator;

class JsonOperation
{
public:
	JsonOperation();
	//JsonOperation(const nlohmann::ordered_json& write);
	explicit JsonOperation(const nlohmann::json& read);
	virtual ~JsonOperation();

	//?!? traits ���϶����� ����� �ø��� ������, �̷��� �ٸ� Ŭ������ ���⸦ �� �� �ֱ� ������ �����Ҷ� private�� �Űܾ� �Ѵ�.
	inline nlohmann::ordered_json& GetWriteRoot() noexcept { return m_write; }
	inline nlohmann::ordered_json& GetWrite() noexcept { return *m_wCurrent; }
	inline nlohmann::json& GetRead() const noexcept { return *m_rCurrent; }

	bool IsWrite();

	bool Write(const wstring& filename);
	bool Read(const wstring& filename);

	template<typename T>
	void Process(const string& key, T& data) noexcept;

	void Write(const string& key, UIComponent* data); //?!? �ؿ� �Լ��� �� �Լ��� template�� ������ ����.
	void Write(const string& key, UINameGenerator* data);

private:
	template <typename ProcessFunc>
	void ProcessWriteKey(const string& key, ProcessFunc processFunc);
	template <typename ProcessFunc>
	void ProcessReadKey(const string& key, ProcessFunc processFunc);
	template <typename WriteFunc, typename ReadFunc>
	void ProcessImpl(const string& key, WriteFunc&& writeFunc, ReadFunc&& readFunc);

	nlohmann::ordered_json m_write{};
	nlohmann::ordered_json* m_wCurrent{ nullptr };

	nlohmann::json m_read{};
	nlohmann::json* m_rCurrent{ nullptr };
	
};

#include "JsonOperation.hpp"

