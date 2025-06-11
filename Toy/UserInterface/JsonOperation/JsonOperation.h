#pragma once

class UIComponent;
class UINameGenerator;

class JsonOperation
{
public:
	JsonOperation();
	explicit JsonOperation(const nlohmann::ordered_json& write);
	explicit JsonOperation(const nlohmann::json& read);
	virtual ~JsonOperation();

	//?!? traits 패턴때문에 여기로 올리긴 했지만, 이러면 다른 클래스가 여기를 쓸 수 있기 때문에 정리할때 private로 옮겨야 한다.
	inline nlohmann::ordered_json& GetWriteRoot() noexcept { return m_write; }
	inline nlohmann::ordered_json& GetWrite() noexcept { return *m_wCurrent; }
	inline nlohmann::json& GetRead() const noexcept { return *m_rCurrent; }

	bool IsWrite();

	bool Write(const wstring& filename);
	bool Read(const wstring& filename);

	template<typename T>
	void Process(const string& key, T& data) noexcept;

private:
	template <typename WriteFunc, typename ReadFunc>
	void ProcessImpl(const string& key, WriteFunc&& writeFunc, ReadFunc&& readFunc);

	nlohmann::ordered_json m_write{};
	nlohmann::ordered_json* m_wCurrent{ nullptr };

	nlohmann::json m_read{};
	nlohmann::json* m_rCurrent{ nullptr };
};

//Json을 저장하고 읽는 함수. hpp에 구현되어 있다.
//template<HasSerializeIO T>
//bool WriteJsonToFile(T& obj, const wstring& filename)
//template<HasSerializeIO T>
//bool ReadJsonFromFile(const wstring& filename, T& obj)

#include "JsonOperation.hpp"

