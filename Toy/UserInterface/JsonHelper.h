#pragma once

struct IRenderer;
class UIComponent;
class UILayout;
class TransformComponent;
enum class Origin;

bool WriteJson(nlohmann::ordered_json& data, const wstring& filename) noexcept;
nlohmann::json ReadJson(const wstring& filename) noexcept;

template<typename T>
concept Primitive = is_arithmetic<T>::value || is_same_v<T, string>;

template<Primitive T>
void DataToJson(const string& key, const T& data, nlohmann::ordered_json& outJson) noexcept
{
	outJson[key] = data;
}

template<typename T>
void DataToJson(const string& key, const unique_ptr<T>& data, nlohmann::ordered_json& outJson) noexcept
{
	outJson[key] = *data;
}

void DataToJson(const string& key, const wstring& data, nlohmann::ordered_json& outJson) noexcept;
void DataToJson(const string& key, const map<wstring, size_t>& data, nlohmann::ordered_json& outJson) noexcept;
void DataToJson(const string& key, const map<wstring, wstring>& data, nlohmann::ordered_json& outJson) noexcept;
void DataToJson(const string& key, const Rectangle& rect, nlohmann::ordered_json& outJson) noexcept;
void DataToJson(const string& key, Origin origin, nlohmann::ordered_json& outJson) noexcept;
void DataToJson(const string& key, const Vector2& vec, nlohmann::ordered_json& outJson) noexcept;
void DataToJson(const string& key, const vector<unique_ptr<TransformComponent>>& data, nlohmann::ordered_json& outJson) noexcept;

template<typename T>
void DataToJson(const vector<T>& data, nlohmann::ordered_json& outJson) noexcept
{
	ranges::for_each(data, [&outJson](const auto& iter) {
		iter.ToJson(outJson);
		});
}

void ComponentToJson(const string& key, const unique_ptr<UIComponent>& component, nlohmann::ordered_json& outJson) noexcept;

/////////////////////////////////////////////////////////////////////////////

template<Primitive T>
void DataFromJson(const string& key, T& outData, const nlohmann::json& j) noexcept
{
	outData = j[key];
}

template<typename T>
void DataFromJson(const string& key, unique_ptr<T>& outData, const nlohmann::json& j) noexcept
{
	*outData = j[key];
}

void DataFromJson(const string& key, Rectangle& outRect, const nlohmann::json& j) noexcept;
void DataFromJson(const string& key, Origin& outOrigin, const nlohmann::json& j) noexcept;
void DataFromJson(const string& key, map<wstring, wstring>& outData, const nlohmann::json& j) noexcept;
void DataFromJson(const string& key, wstring& outData, const nlohmann::json& j) noexcept;
void DataFromJson(const string& key, Vector2& outData, const nlohmann::json& j) noexcept;
void DataFromJson(const string& key, vector<unique_ptr<TransformComponent>>& outData, const nlohmann::json& j);

void ComponentFromJson(const string& key, unique_ptr<UIComponent>& component, const nlohmann::json& j);
