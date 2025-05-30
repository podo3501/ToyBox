#pragma once
#include "JsonDetail.hpp"

//Traits를 간단하게 선언하는 코드. 템플릿 특수화가 타입별로 선언이 계속 되어야 하기 때문에 Define으로 선언한다.
#ifndef DECLARE_JSON_TRAITS
#define DECLARE_JSON_TRAITS(Type) \
template<> \
struct JsonTraitsBase<Type> \
{ \
    static nlohmann::ordered_json SerializeToJson(const Type&); \
    static Type DeserializeFromJson(const nlohmann::json&); \
};
#endif // DECLARE_JSON_TRAITS

template<typename T>
struct JsonTraitsBase
{
	static nlohmann::ordered_json SerializeToJson(const T& data) { return data; }
	static T DeserializeFromJson(const nlohmann::json& dataJ) { return dataJ; }
};

template<typename T>
struct JsonTraits : public JsonTraitsBase<T> {};

enum class Origin;

DECLARE_JSON_TRAITS(XMINT2)
DECLARE_JSON_TRAITS(XMUINT2)
DECLARE_JSON_TRAITS(Rectangle)
DECLARE_JSON_TRAITS(Origin)
DECLARE_JSON_TRAITS(Vector2)
DECLARE_JSON_TRAITS(string)
DECLARE_JSON_TRAITS(wstring)


