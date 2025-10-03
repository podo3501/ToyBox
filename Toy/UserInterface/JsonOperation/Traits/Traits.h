#pragma once
#include "JsonDetail.hpp"

//Traits�� �����ϰ� �����ϴ� �ڵ�. ���ø� Ư��ȭ�� Ÿ�Ժ��� ������ ��� �Ǿ�� �ϱ� ������ Define���� �����Ѵ�.
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

DECLARE_JSON_TRAITS(XMINT2)
DECLARE_JSON_TRAITS(XMUINT2)
DECLARE_JSON_TRAITS(Rectangle)
DECLARE_JSON_TRAITS(Vector2)
DECLARE_JSON_TRAITS(string)
DECLARE_JSON_TRAITS(wstring)


