#pragma once

namespace json_detail
{
	// Key ��ȯ ����
	template<typename K>
	struct KeyConverter {
		static string ToKey(const K& key) { return key; }
		static K FromKey(const string& key) { return key; }
	};

	// Ư��ȭ: wstring ó��
	template<>
	struct KeyConverter<wstring> {
		static string ToKey(const wstring& key);
		static wstring FromKey(const string& key);
	};

	template<>
	struct KeyConverter<int> {
		static string ToKey(const int& key);
		static int FromKey(const string& key);
	};

	// ��ȯ �Լ� ����
	template<typename K> string ToKeyString(const K& key) { return KeyConverter<K>::ToKey(key); }
	template<typename K> K FromKeyString(const string& key) { return KeyConverter<K>::FromKey(key); }

	template<typename T, typename J>
	static void SafeRead(T& out, const J& value)
	{
		Assert(!value.is_null());
		out = value.get<T>();
	}
	// x, y ���� ó�� ����
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

	// Rectangle ���� �߰� �ʵ� ó�� ����
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
