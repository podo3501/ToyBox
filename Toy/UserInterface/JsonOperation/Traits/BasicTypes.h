#pragma once
#include "Traits.hpp"
#include "JsonDetail.hpp"
#include "../../UIComponent/UIType.h"
#include "../../../Utility.h"

//template<typename T>
//struct JsonSerializable
//{
//	static nlohmann::ordered_json SerializeToJson(T& data);
//	static T DeserializeFromJson(const nlohmann::json& dataJ);
//};

//template<>
//struct JsonTraits<XMINT2> : public JsonTraitsBase<XMINT2, JT_XMINT2> {};

//template<>
//struct JsonTraits<XMINT2> : public JsonSerializable<XMINT2> 
//{
//	static nlohmann::ordered_json SerializeToJson(XMINT2& data) { return json_detail::SerializeXY(data); }
//	static XMINT2 DeserializeFromJson(const nlohmann::json& dataJ)
//	{
//		XMINT2 data{};
//		json_detail::DeserializeXY(data, dataJ);
//		return data;
//	}
//};

//template<>
//struct JsonTraits<XMINT2>
//{
//	static nlohmann::ordered_json SerializeToJson(XMINT2& data);
//	static XMINT2 DeserializeFromJson(const nlohmann::json& dataJ);
//};
//
//template<>
//struct JsonTraits<XMUINT2>
//{
//	static nlohmann::ordered_json SerializeToJson(XMUINT2& data);
//	static XMUINT2 DeserializeFromJson(const nlohmann::json& dataJ);
//};
//
//template<>
//struct JsonTraits<Rectangle>
//{
//	static nlohmann::ordered_json SerializeToJson(Rectangle& data);
//	static Rectangle DeserializeFromJson(const nlohmann::json& dataJ);
//};
//
//template<>
//struct JsonTraits<Origin>
//{
//	static nlohmann::ordered_json SerializeToJson(Origin& data);
//	static Origin DeserializeFromJson(const nlohmann::json& dataJ);
//};
//
//template<>
//struct JsonTraits<Vector2>
//{
//	static nlohmann::ordered_json SerializeToJson(Vector2& data);
//	static Vector2 DeserializeFromJson(const nlohmann::json& dataJ);
//};
//
//template<>
//struct JsonTraits<wstring>
//{
//	static nlohmann::ordered_json SerializeToJson(wstring& data);
//	static wstring DeserializeFromJson(const nlohmann::json& dataJ);
//};