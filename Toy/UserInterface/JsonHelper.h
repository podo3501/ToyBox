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

DataType GetType(const string& key);
tuple<unique_ptr<UIComponent>, Vector2> CreateComponent(const nlohmann::json& data);
nlohmann::json LoadUIFile(const wstring& filename);
bool WriteJson(const nlohmann::json& data, const wstring& filename);
