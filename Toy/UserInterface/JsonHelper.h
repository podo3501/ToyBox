#pragma once

struct IRenderer;
class UIComponent;
class UILayout;
enum class Origin;

enum class DataType
{
	Init = 0,
	Name,	//Component의 이름
	Layout,	//위치, 정렬정보
	Component,	//Dialog, Button 같은 붙일수 있는 것들
	Property,	//Component안에 들어있는 속성들(ex 이미지)
};

enum class ComponentType
{
	Init = 0,
	Dialog,
	BGImage,
};

DataType GetType(const string& key);
tuple<unique_ptr<UIComponent>, Vector2> CreateComponent(const nlohmann::json& data);
nlohmann::json LoadUIFile(const wstring& filename);
bool WriteJsonAA(const nlohmann::ordered_json& data, const wstring& filename) noexcept;

void DataToJson(const string& key, const string& data, nlohmann::ordered_json& outJson) noexcept;
void DataToJson(const string& key, const wstring& data, nlohmann::ordered_json& outJson) noexcept;
void DataToJson(const string& key, const map<wstring, size_t>& data, nlohmann::ordered_json& outJson) noexcept;
void DataToJson(const string& key, const map<wstring, wstring>& data, nlohmann::ordered_json& outJson) noexcept;
void DataToJson(const string& key, const Rectangle& rect, nlohmann::ordered_json& outJson) noexcept;
void DataToJson(const string& key, Origin origin, nlohmann::ordered_json& outJson) noexcept;

template<typename T>
void DataToJson(const vector<T>& data, nlohmann::ordered_json& outJson) noexcept
{
	ranges::for_each(data, [&outJson](const auto& iter) {
		iter.ToJson(outJson);
		});
}

nlohmann::json ReadJsonAA(const wstring& filename) noexcept;

void DataFromJson(const string& key, string& outStr, const nlohmann::json& j) noexcept;
void DataFromJson(const string& key, Rectangle& outRect, const nlohmann::json& j) noexcept;
void DataFromJson(const string& key, Origin& outOrigin, const nlohmann::json& j) noexcept;
void DataFromJson(const string& key, map<wstring, wstring>& outData, const nlohmann::json& j) noexcept;
void DataFromJson(const string& key, wstring& outData, const nlohmann::json& j) noexcept;
