#pragma once

template<typename T> requires is_default_constructible_v<T>&& is_copy_assignable_v<T>
class Property;
class UIComponent;
class UINameGenerator;
class ComponentNameGenerator;
class Scene;
class UITransform;
struct TextureSourceInfo;
struct TextureFontInfo;
enum class Origin;//enum은 전방선언후 컨셉에 추가만 하면 된다.
enum class InteractState; 
enum class DirectionType;
enum class BehaviorMode;
enum class TextureSlice;
enum class RenderTraversal;
namespace StateFlag { enum Type : int; }

namespace Tool
{
	enum class ResolutionType;
}

template <typename T>
class JsonNavigator {
public:
	// 초기 JSON 객체 설정 (unique_ptr로 관리)
	explicit JsonNavigator(unique_ptr<T> root)
		: root(move(root)), current(this->root.get()) {}

	// 특정 키로 내려가기 (키가 없으면 생성)
	bool GotoKey(const string& key, bool createArray = false)
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
	T& GetCurrent() const noexcept {
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
	unique_ptr<T> root;         // 루트 객체
	T* current;                      // 현재 탐색 중인 객체
	std::stack<T*> parentStack;      // 부모 객체 추적을 위한 스택
};

// STL 컨테이너인지 확인하는 메타함수
template <typename T>
struct is_stl_container : false_type {};

// STL 컨테이너 특수화
template <typename... Args>
struct is_stl_container<vector<Args...>> : true_type {};

template <typename... Args>
struct is_stl_container<list<Args...>> : true_type {};

template <typename... Args>
struct is_stl_container<deque<Args...>> : true_type {};

template<typename T>
concept IsClassType = is_class_v<T>;

template<typename T>	//기본 타입 및 enum 타입
concept IsBasicType =
is_arithmetic_v<T> ||
is_same_v<T, string> ||
is_same_v<T, size_t> ||
is_same_v<T, StateFlag::Type> ||
is_same_v <T, InteractState> ||
is_same_v <T, DirectionType> ||
is_same_v <T, TextureSlice> ||
is_same_v <T, BehaviorMode> ||
is_same_v <T, RenderTraversal> ||
is_same_v<T, Tool::ResolutionType>;

template<typename T>
concept IsBasicContainer =
is_stl_container<T>::value &&
!IsClassType<typename T::value_type>;

template<typename T>
concept IsClassContainer =
is_stl_container<T>::value &&
IsClassType<typename T::value_type>;

template<typename T>
concept IsClass =
!IsBasicType<T> &&
!is_stl_container<T>::value &&
!requires {                     // Map과 같은 연관 컨테이너 제외
	typename T::key_type;
	typename T::mapped_type;
} &&
IsClassType<T>;

//Json이 stl에 어떤 것을 지원하는지 검색하고 지원이 된다면 stl 컨테이너 특수화에 추가해서 
//기본 Process를 사용해서 되는지 먼저 확인후, 지원이 안된다면 Process함수를 추가하자.
template<typename T>
concept Available =
IsBasicType<T> || IsBasicContainer<T>;

template<typename T>
concept IsNotUIComponent = !std::is_same_v<T, UIComponent>;

class JsonOperation
{
public:
	JsonOperation();
	//JsonOperation(const nlohmann::ordered_json& write);
	explicit JsonOperation(const nlohmann::json& read);
	virtual ~JsonOperation();

	//?!? traits 패턴때문에 여기로 올리긴 했지만, 이러면 다른 클래스가 여기를 쓸 수 있기 때문에 정리할때 private로 옮겨야 한다.
	inline nlohmann::ordered_json& GetWrite() noexcept { return m_write->GetCurrent(); }
	inline nlohmann::json& GetRead() const noexcept { return m_read->GetCurrent(); }

	bool IsWrite();

	bool Write(const wstring& filename);
	bool Read(const wstring& filename);

	template<Available T>
	void Process(const string& key, T& data) noexcept;
	template<IsClass T>
	void Process(const string& key, T& data) noexcept;
	template<IsNotUIComponent T>
	void Process(const string& key, unique_ptr<T>& data);
	template<IsClassContainer T>
	void Process(const string& key, T& data) noexcept;
	template<typename T>
	void Process(const string& key, vector<T>& datas);
	template<typename T>
	void Process(const string& key, vector<unique_ptr<T>>& data);
	template<typename T>
	void Process(const string& key, Property<T>& data);
	template<typename K, typename T>
	void Process(const string& key, unordered_map<K, T>& datas) noexcept;
	/*template<typename T>
		requires requires (T& t, const nlohmann::json& j) {
			{ JsonTraits<T>::SerializeToJson(t) } -> same_as<nlohmann::ordered_json>;
			{ JsonTraits<T>::DeserializeFromJson(j) } -> same_as<T>;
	}
	void Process(const string& key, T& data) noexcept;*/

	void Process(const string& key, XMINT2& data) noexcept;
	void Process(const string& key, XMUINT2& data) noexcept;
	void Process(const string& key, Rectangle& data) noexcept;
	void Process(const string& key, Origin& data) noexcept; //기본 템플릿에 넣어도 되나, 저장할때 string으로 저장하면 보기가 좋다.
	void Process(const string& key, Vector2& data) noexcept;
	void Process(const string& key, unique_ptr<UIComponent>& data);
	void Process(const string& key, vector<unique_ptr<UIComponent>>& datas);
	void Process(const string& key, wstring& data) noexcept;
	//?!? map을 template로 바꿀 차례
	void Process(const string& key, map<int, UITransform>& datas) noexcept;
	void Process(const string& key, map<InteractState, string>& datas) noexcept;
	void Process(const string& key, deque<wstring>& data) noexcept;
	void Write(const string& key, UIComponent* data);
	void Write(const string& key, UINameGenerator* data);

private: //이 함수는 JsonOperation_
	//template<typename T>
	//static nlohmann::ordered_json SerializeToJson(T& value);
	//template<> nlohmann::ordered_json SerializeToJson<XMUINT2>(nlohmann::ordered_json& j, XMUINT2& data);

	template<typename T>
	T DeserializeFromJson(const nlohmann::json& v);
	//template<> void DeserializeFromJson<XMUINT2>(const nlohmann::json& j, XMUINT2& data);

private:
	template<typename T>
	unique_ptr<T> CreateData(const nlohmann::json& readJ = nullptr);
	template <typename ProcessFunc>
	void ProcessWriteKey(const string& key, ProcessFunc processFunc);
	template <typename ProcessFunc>
	void ProcessReadKey(const string& key, ProcessFunc processFunc);
	template <typename WriteFunc, typename ReadFunc>
	void ProcessImpl(const string& key, WriteFunc&& writeFunc, ReadFunc&& readFunc);

	void UpdateJson(const unique_ptr<UIComponent>& data) noexcept;
	void UpdateJson(UIComponent* data) noexcept;
	void UpdateJson(UINameGenerator* data) noexcept;
	unique_ptr<UIComponent> CreateComponentFromType(const string& typeName);

	unique_ptr<JsonNavigator<nlohmann::ordered_json>> m_write;
	unique_ptr<JsonNavigator<nlohmann::json>> m_read;
};

#include "JsonOperation_traits.hpp"
#include "JsonOperation.hpp"

