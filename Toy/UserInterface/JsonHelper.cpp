#include "pch.h"
#include "JsonHelper.h"
#include "../Include/IRenderer.h"
#include "../Utility.h"
#include "Dialog.h"
#include "BGImage.h"
#include "ImagePartSet.h"
#include "UILayout.h"
#include "UIType.h"

using json = nlohmann::json;

unique_ptr<IRenderItem> CreateComponent(ComponentType compType)
{
	switch (compType)
	{
		case ComponentType::Dialog: return make_unique<Dialog>();
		case ComponentType::BGImage: return make_unique<BGImage>();
		case ComponentType::ImagePartSet: return make_unique<ImagePartSet>();
	}

	return nullptr;
}

ComponentType GetComponentType(const string& key)
{
	if (key == "Dialog") return ComponentType::Dialog;
	if (key == "BGImage") return ComponentType::BGImage;
	if (key == "ImagePartSet") return ComponentType::ImagePartSet;

	return ComponentType::Init;
}

tuple<DataType, ComponentType> GetType(const string& key)
{
	DataType dataType{ DataType::Init };
	ComponentType compType{ ComponentType::Init };

	if (key == "Layout") dataType = DataType::Layout;
	compType = GetComponentType(key);
	if (compType != ComponentType::Init)
		dataType = DataType::Component;
	
	return make_tuple(dataType, compType);
}

tuple<unique_ptr<IRenderItem>, Vector2> GetComponent(ComponentType compType, const json& data)
{
	wstring compFilename{ StringToWString(data["Filename"]) };
	const auto& pos = data["Position"];
	Vector2 position{ pos["x"], pos["y"] };

	unique_ptr<IRenderItem> item = CreateComponent(compType);
	if (item->SetResources(compFilename) == false)
		return make_tuple(nullptr, position);

	return make_tuple(move(item), position);
}