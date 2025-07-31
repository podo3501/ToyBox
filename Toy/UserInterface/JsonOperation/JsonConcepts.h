#pragma once
#include <concepts>
#include <type_traits>
#include <string>

// SerializeIO ���� ���� Ȯ��
class JsonOperation;
template<typename T>
concept HasSerializeIO =
	requires(T t, JsonOperation & op) {
		{ t->SerializeIO(op) };   // ����Ʈ �����Ϳ�
	} ||
	requires(T t, JsonOperation & op) {
		{ t.SerializeIO(op) };    // �Ϲ� ��ü��
	};

template<typename T> //��� begin/end�� �ִ� ���
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
!requires { typename T::key_type; }&&     // key_type ����� �� (map ����)
	requires { typename T::value_type; }&&    // value_type �־�� �� (sequence ����)
HasMemberBeginEnd<T> &&
NotBasicString<T>;    // ���ڿ� �迭 ����

template<typename T>
concept MapLike = requires {
	typename T::key_type;
	typename T::mapped_type; }&&
	HasMemberBeginEnd<T>;

template<typename T> //�̰��� unique, shared �� ã���ִ°� �ƴ϶� element_type �� �ִ� ���� ã���ִ� �ǵ�, ��κ� �� Ÿ���� ������ smartPtr�� �ֱ� �����̴�.
concept SmartPointerLike = requires {
	typename T::element_type; };