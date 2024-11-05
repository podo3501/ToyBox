#pragma once

struct IRenderer;
struct IRenderItem;
class UILayout;

enum class DataType
{
	Init = 0,
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
tuple<unique_ptr<IRenderItem>, Vector2> GetComponent(const nlohmann::json& data);
tuple<wstring, Vector2> GetFilenameAndPos(const nlohmann::json& data);
nlohmann::json LoadUIFile(const wstring& filename);

template<typename T>
tuple<unique_ptr<T>, Vector2> GetProperty(const nlohmann::json& data)
{
	auto [filename, position] = GetFilenameAndPos(data);
	auto property = make_unique<T>();
	auto result = property->SetResources(filename);
	if (!result)
		return { nullptr, Vector2{} };
	return make_tuple(move(property), position);
}
