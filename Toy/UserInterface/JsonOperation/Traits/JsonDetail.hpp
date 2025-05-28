#pragma once

//키 변환을 간단하게 선언하는 Define 코드. 키가 템플릿 특수화로 돼 있어서 똑같은 선언을 계속 해야 하기 때문.
#ifndef DECLARE_KEY_CONVERTER
#define DECLARE_KEY_CONVERTER(Type)                 \
	template<>                                         \
	struct KeyConverter<Type> {                        \
		static string ToKey(const Type&);     \
		static Type FromKey(const string&);   \
	};
#endif //DECLARE_KEY_CONVERTER

enum class InteractState;
enum class ComponentID;
namespace json_detail
{
	//Key 변환 헬퍼
	template<typename K>
	struct KeyConverter {
		static string ToKey(const K& key) { return key; }
		static K FromKey(const string& key) { return key; }
	};

	//Key 특수화
	DECLARE_KEY_CONVERTER(int)
	DECLARE_KEY_CONVERTER(wstring)
	DECLARE_KEY_CONVERTER(InteractState)
	DECLARE_KEY_CONVERTER(ComponentID)

	template<typename K> string ToKeyString(const K& key) { return KeyConverter<K>::ToKey(key); }
	template<typename K> K FromKeyString(const string& key) { return KeyConverter<K>::FromKey(key); }

	template<typename T, typename J>
	static void SafeRead(T& out, const J& value)
	{
		Assert(!value.is_null());
		out = value.get<T>();
	}
	// x, y 공통 처리 헬퍼
	template<typename T>
	nlohmann::ordered_json SerializeXY(const T& data)
	{
		nlohmann::ordered_json j;
		j["x"] = data.x;
		j["y"] = data.y;
		return j;
	}

	template<typename T>
	void DeserializeXY(T& data, const nlohmann::json& j)
	{
		SafeRead(data.x, j["x"]);
		SafeRead(data.y, j["y"]);
	}

	template<typename T, typename Func>
	T CreateAndFill(Func&& func)
	{
		T data{};
		forward<Func>(func)(data);
		return data;
	}

	// Rectangle 전용 추가 필드 처리 헬퍼
	inline nlohmann::ordered_json SerializeRectExtra(const Rectangle& data, nlohmann::ordered_json j)
	{
		j["width"] = data.width;
		j["height"] = data.height;
		return j;
	}

	inline void DeserializeRectExtra(Rectangle& data, const nlohmann::json& j)
	{
		SafeRead(data.width, j["width"]);
		SafeRead(data.height, j["height"]);
	}
}
