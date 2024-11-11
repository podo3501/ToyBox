#include "pch.h"
#include "JsonHelper.h"
#include "../Include/IRenderer.h"
#include "../Utility.h"
#include "Dialog.h"
#include "BGImage.h"
#include "ImagePartSet.h"
#include "UILayout.h"
#include "UIType.h"
#include "../Config.h"

using json = nlohmann::json;

unique_ptr<UIComponent> CreateComponent(ComponentType compType)
{
	switch (compType)
	{
		case ComponentType::Dialog: return make_unique<Dialog>();
		case ComponentType::BGImage: return make_unique<BGImage>();
	}

	return nullptr;
}

ComponentType GetComponentType(const string& key)
{
	if (key == "Dialog") return ComponentType::Dialog;
	if (key == "BGImage") return ComponentType::BGImage;

	return ComponentType::Init;
}

DataType GetType(const string& key)
{
	DataType dataType{ DataType::Init };

	if (key == "Name") dataType = DataType::Name;
	if (key == "Layout") dataType = DataType::Layout;
	if (key == "Property") dataType = DataType::Property;
	if (key == "Component") dataType = DataType::Component;
	
	return dataType;
}

wstring GetFilename(const nlohmann::json& data)
{
	return { StringToWString(data["Filename"]) };
}

Vector2 GetPosition(const nlohmann::json& data)
{
	const auto& pos = data["Position"];
	return { pos["x"], pos["y"] };
}

tuple<wstring, Vector2> GetFilenameAndPos(const json& data)
{
	return make_tuple(GetFilename(data), GetPosition(data));
}

tuple<unique_ptr<UIComponent>, Vector2> CreateComponent(const json& data)
{
	ComponentType compType = GetComponentType(data["Type"]);
	const auto& [compFilename, position] = GetFilenameAndPos(data);

	unique_ptr<UIComponent> comp = CreateComponent(compType);
	if (comp->SetResources(compFilename) == false)
		return make_tuple(nullptr, position);

	return make_tuple(move(comp), position);
}

json LoadUIFile(const wstring& filename)
{
	ifstream file(GetResourcePath() + filename);
	if (!file.is_open())
		return nullptr;

	return json::parse(file);
}