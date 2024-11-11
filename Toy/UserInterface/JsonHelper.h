#pragma once

struct IRenderer;
class UIComponent;
class UILayout;

enum class DataType
{
	Init = 0,
	Name,	//Component�� �̸�
	Layout,	//��ġ, ��������
	Component,	//Dialog, Button ���� ���ϼ� �ִ� �͵�
	Property,	//Component�ȿ� ����ִ� �Ӽ���(ex �̹���)
};

enum class ComponentType
{
	Init = 0,
	Dialog,
	BGImage,
};

enum class PropertyType
{
	ImagePartSet	//ImageList�� �ٲ���
};

DataType GetType(const string& key);
tuple<unique_ptr<UIComponent>, Vector2> CreateComponent(const nlohmann::json& data);
tuple<wstring, Vector2> GetFilenameAndPos(const nlohmann::json& data);
nlohmann::json LoadUIFile(const wstring& filename);

template<typename T>
tuple<unique_ptr<T>, Vector2> CreateProperty(const nlohmann::json& data)
{
	auto [filename, position] = GetFilenameAndPos(data);
	auto property = make_unique<T>();
	auto result = property->SetResources(filename);
	if (!result)
		return { nullptr, Vector2{} };
	return make_tuple(move(property), position);
}
