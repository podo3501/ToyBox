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

unique_ptr<IRenderItem> CreateComponent(ComponentType compType)
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

	if (key == "Layout") dataType = DataType::Layout;
	if (key == "Property") dataType = DataType::Property;
	if (key == "Component") dataType = DataType::Component;
	
	return dataType;
}

tuple<wstring, Vector2> GetFilenameAndPos(const json& data)
{
	wstring filename{ StringToWString(data["Filename"]) };
	const auto& pos = data["Position"];
	Vector2 position{ pos["x"], pos["y"] };

	return make_tuple(filename, position);
}

tuple<unique_ptr<IRenderItem>, Vector2> GetComponent(const json& data)
{
	ComponentType compType = GetComponentType(data["Type"]);
	const auto& [compFilename, position] = GetFilenameAndPos(data);

	unique_ptr<IRenderItem> item = CreateComponent(compType);
	if (item->SetResources(compFilename) == false)
		return make_tuple(nullptr, position);

	return make_tuple(move(item), position);
}

json LoadUIFile(const wstring& filename)
{
	ifstream file(GetResourcePath() + filename);
	if (!file.is_open())
		return nullptr;

	return json::parse(file);
}