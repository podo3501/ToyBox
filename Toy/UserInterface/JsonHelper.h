#pragma once

struct IRenderer;
class UIComponent;
class UILayout;

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

enum class PropertyType
{
	ImagePartSet	//ImageList로 바꾸자
};

DataType GetType(const string& key);
tuple<unique_ptr<UIComponent>, Vector2> CreateComponent(const nlohmann::json& data);
tuple<wstring, Vector2> GetFilenameAndPos(const nlohmann::json& data);
wstring GetFilename(const nlohmann::json& data);
nlohmann::json LoadUIFile(const wstring& filename);

template<typename T>
unique_ptr<T> CreateProperty(const nlohmann::json& data)
{
	auto property = make_unique<T>();
	auto result = property->SetResources(GetFilename(data));
	if(!result)
		return nullptr;

	return move(property);
}
