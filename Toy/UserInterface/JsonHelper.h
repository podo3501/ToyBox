#pragma once

struct IRenderer;
struct IRenderItem;
class UILayout;

enum class DataType
{
	Init = 0,
	Layout,
	Component
};

enum class ComponentType
{
	Init = 0,
	Dialog,
	BGImage,
	ImagePartSet
};

tuple<DataType, ComponentType> GetType(const string& key);
tuple<unique_ptr<IRenderItem>, Vector2> GetComponent(ComponentType compType, const nlohmann::json& data);
