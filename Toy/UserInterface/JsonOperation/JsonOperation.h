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

// SerializeIO 지원 여부 확인
class JsonOperation;
template<typename T>
concept HasSerializeIO = requires(T t, JsonOperation & op) {
	{ t.SerializeIO(op) };
};

class JsonOperation
{
public:
	JsonOperation();
	//JsonOperation(const nlohmann::ordered_json& write);
	explicit JsonOperation(const nlohmann::json& read);
	virtual ~JsonOperation();

	//?!? traits 패턴때문에 여기로 올리긴 했지만, 이러면 다른 클래스가 여기를 쓸 수 있기 때문에 정리할때 private로 옮겨야 한다.
	inline nlohmann::ordered_json& GetWriteRoot() noexcept { return m_write; }
	inline nlohmann::ordered_json& GetWrite() noexcept { return *m_wCurrent; }
	inline nlohmann::json& GetRead() const noexcept { return *m_rCurrent; }

	bool IsWrite();

	bool Write(const wstring& filename);
	bool Read(const wstring& filename);

	template<Available T>
	void Process(const string& key, T& data) noexcept;
	template<IsClass T>
	void Process(const string& key, T& data) noexcept;
	template<IsClassContainer T>
	void Process(const string& key, T& data) noexcept;
	template<IsNotUIComponent T>
	void Process(const string& key, unique_ptr<T>& data);
	//template<typename T>
	//void Process(const string& key, vector<T>& datas);
	template<typename Container>
	void Process(const string& key, Container& datas) noexcept;
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
	void Process(const string& key, deque<wstring>& data) noexcept;
	void Write(const string& key, UIComponent* data); //?!? 밑에 함수와 이 함수는 template로 합쳐질 예정.
	void Write(const string& key, UINameGenerator* data);

private: //이 함수는 JsonOperation_
	//template<typename T>
	//static nlohmann::ordered_json SerializeToJson(T& value);
	//template<> nlohmann::ordered_json SerializeToJson<XMUINT2>(nlohmann::ordered_json& j, XMUINT2& data);

	template<typename T>
	T DeserializeFromJson(const nlohmann::json& v);
	//template<> void DeserializeFromJson<XMUINT2>(const nlohmann::json& j, XMUINT2& data);

private:
	template <typename ProcessFunc>
	void ProcessWriteKey(const string& key, ProcessFunc processFunc);
	template <typename ProcessFunc>
	void ProcessReadKey(const string& key, ProcessFunc processFunc);
	template <typename WriteFunc, typename ReadFunc>
	void ProcessImpl(const string& key, WriteFunc&& writeFunc, ReadFunc&& readFunc);

	unique_ptr<UIComponent> CreateComponentFromType(const string& typeName, const nlohmann::json& readJ);

	nlohmann::ordered_json m_write{};
	nlohmann::ordered_json* m_wCurrent{ nullptr };

	nlohmann::json m_read{};
	nlohmann::json* m_rCurrent{ nullptr };
	
};

#include "JsonOperation.hpp"

