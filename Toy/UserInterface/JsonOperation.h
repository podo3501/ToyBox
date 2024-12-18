#pragma once

class UIComponent;
class Scene;
enum class Origin;
enum class ButtonState;

namespace Tool
{
	enum class ResolutionType;
}

template <typename T>
class JsonNavigator {
public:
	// 초기 JSON 객체 설정 (unique_ptr로 관리)
	explicit JsonNavigator(std::unique_ptr<T> root)
		: root(std::move(root)), current(this->root.get()) {}

	// 특정 키로 내려가기 (키가 없으면 생성)
	bool GotoKey(const std::string& key, bool createArray = false)
	{
		if (!current || key.empty()) return false;

		if (!current->contains(key)) {
			(*current)[key] = createArray ? T::array() : T();
		}

		parentStack.push(current);
		current = &(*current)[key];
		return true;
	}

	// 부모 객체로 올라가기
	bool GoBack() {
		if (!parentStack.empty()) {
			current = parentStack.top();     // 부모 객체로 돌아가기
			parentStack.pop();               // 스택에서 부모 객체 제거
			return true;
		}
		return false;
	}

	// 현재 객체 값 가져오기
	T& GetCurrent() {
		return *current;
	}

	//json을 읽을때 특정 json으로 읽어야 할 때가 있다. 그때 살짝 자리바꿈 한 다음에 goBack으로 돌려놓는다.
	void SetCurrent(T* _current)
	{
		current = _current;
	}

	bool IsEmpty()
	{
		return current->empty();
	}

	//// 현재 객체 값 설정
	//template <typename ValueType>
	//void setValue(const ValueType& value) {
	//    *current = value;
	//}

	//// 특정 키에 값 설정
	//template <typename ValueType>
	//void SetKeyValue(const std::string& key, const ValueType& value) {
	//	(*current)[key] = value;
	//}

	// 루트 JSON 반환 (상태 확인용)
	const T& GetRoot() const {
		return *root;
	}

private:
	std::unique_ptr<T> root;         // 루트 객체
	T* current;                      // 현재 탐색 중인 객체
	std::stack<T*> parentStack;      // 부모 객체 추적을 위한 스택
};

// STL 컨테이너인지 확인하는 메타함수
template <typename T>
struct is_stl_container : std::false_type {};

// STL 컨테이너 특수화
template <typename... Args>
struct is_stl_container<std::vector<Args...>> : std::true_type {};

template <typename... Args>
struct is_stl_container<std::list<Args...>> : std::true_type {};

template <typename... Args>
struct is_stl_container<std::deque<Args...>> : std::true_type {};

template<typename T>
concept IsClassContainer =
	is_stl_container<T>::value &&
	std::is_class_v<typename T::value_type>;

//Json이 stl에 어떤 것을 지원하는지 검색하고 지원이 된다면 stl 컨테이너 특수화에 추가해서 
//기본 Process를 사용해서 되는지 먼저 확인후, 지원이 안된다면 Process함수를 추가하자.
template<typename T>
concept Available = 
	is_arithmetic<T>::value ||
	is_same_v<T, string> ||
	is_same_v<T, size_t> ||
	is_same_v<T, Tool::ResolutionType> ||
	is_stl_container<T>::value &&	// stl 컨테이너가 기본(int같은)형일때 그리고 
	!std::is_class_v<typename T::value_type>;	//그 데이터가 클래스나 스트럭쳐가 아닌경우

template<typename T>
concept IsNotUIComponent = !std::is_same_v<T, UIComponent>;

class JsonOperation
{
public:
	JsonOperation();
	JsonOperation(const nlohmann::json& read);
	virtual ~JsonOperation();

	bool IsWrite();

	bool Write(const wstring& filename);
	bool Read(const wstring& filename);

	template<Available T>
	void Process(const string& key, T& data) noexcept;
	template<IsNotUIComponent T>
	void Process(const string& key, unique_ptr<T>& data);
	template<IsClassContainer T>
	void Process(const string& key, T& data) noexcept;
	template<typename T>
	void Process(const string& key, vector<unique_ptr<T>>& data);

	void Process(const string& key, Rectangle& data) noexcept;
	void Process(const string& key, Origin& data) noexcept;
	void Process(const string& key, Vector2& data) noexcept;
	void Process(const string& key, unique_ptr<UIComponent>& data);
	void Process(const string& key, wstring& data) noexcept;
	void Process(const string& key, map<wstring, wstring>& data) noexcept;
	void Process(const string& key, deque<wstring>& data) noexcept;

private:
	template<typename T>
	unique_ptr<T> CreateData(const nlohmann::json& readJ = nullptr);
	template <typename ProcessFunc>
	void ProcessWriteKey(const string& key, ProcessFunc processFunc);
	template <typename ProcessFunc>
	void ProcessReadKey(const string& key, ProcessFunc processFunc);

	nlohmann::ordered_json& GetWrite();
	nlohmann::json& GetRead();

	void UpdateJson(const unique_ptr<UIComponent>& data) noexcept;
	unique_ptr<UIComponent> CreateComponentFromType(const string& typeName);
	void ProcessImpl(const string& key, auto readFunc, auto writeFunc);

	unique_ptr<JsonNavigator<nlohmann::ordered_json>> m_write;
	unique_ptr<JsonNavigator<nlohmann::json>> m_read;
};

#include "JsonOperation.hpp"

