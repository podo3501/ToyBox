#pragma once

// SerializeIO 지원 여부 확인
class JsonOperation;
template<typename T>
concept HasSerializeIO =
	requires(T t, JsonOperation & op) {
		{ t->SerializeIO(op) };   // 스마트 포인터용
	} ||
	requires(T t, JsonOperation & op) {
		{ t.SerializeIO(op) };    // 일반 객체용
	};

//template<typename T>
//concept HasSerializeIOSmartPtr =
//(requires { typename T::element_type; }) &&  // 스마트 포인터 개념 탐지
//HasSerializeIO<typename T::element_type>;

template<typename T>
concept SmartPtr = requires(T p) {
	typename T::element_type;
	{ p.get() } -> std::same_as<typename T::element_type*>;
};

template<typename T> //멤버 begin/end가 있는 경우
concept HasMemberBeginEnd = requires(T t) {
	{ t.begin() } -> std::input_iterator;
	{ t.end() } -> std::sentinel_for<decltype(t.begin())>;
};

template<typename T>
concept NotBasicString =
!(std::same_as<std::remove_cvref_t<T>, std::string> ||
	std::same_as<std::remove_cvref_t<T>, std::wstring> ||
	std::same_as<std::remove_cvref_t<T>, std::u16string> ||
	std::same_as<std::remove_cvref_t<T>, std::u32string>);

template<typename T>
concept SequenceLike =
!requires { typename T::key_type; }&&     // key_type 없어야 함 (map 방지)
	requires { typename T::value_type; }&&    // value_type 있어야 함 (sequence 조건)
HasMemberBeginEnd<T> &&
NotBasicString<T>;    // 문자열 계열 제외

template<typename T>
concept MapLike = requires {
	typename T::key_type;
	typename T::mapped_type; }&&
	HasMemberBeginEnd<T>;