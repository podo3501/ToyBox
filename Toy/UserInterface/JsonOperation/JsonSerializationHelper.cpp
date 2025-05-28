#include "pch.h"
#include "JsonSerializationHelper.hpp"
#include "../UIComponent/UIComponent.h"
#include "../UIComponent/UIComponentFactory.h"
#include "JsonOperation.h"

void SerializeClassIO_Internal(UIComponent& data, nlohmann::ordered_json& j)
{
	j["Type"] = EnumToString<ComponentID>(data.GetTypeID());
	j.update(SerializeClassIO_GenerateJson(data));
}

void DeserializeClassIO(const nlohmann::json& j, unique_ptr<UIComponent>& data)
{
	string curType{ j["Type"] };
	unique_ptr<UIComponent> comp = CreateComponent(curType);
	DeserializeClassIO_Internal(j, *comp);
	data = move(comp);
}