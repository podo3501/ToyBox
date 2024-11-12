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

DataType GetType(const string& key);
tuple<unique_ptr<UIComponent>, Vector2> CreateComponent(const nlohmann::json& data);
nlohmann::json LoadUIFile(const wstring& filename);
bool WriteJson(const nlohmann::json& data, const wstring& filename);
