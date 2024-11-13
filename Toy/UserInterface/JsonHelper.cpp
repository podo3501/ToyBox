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
using ordered_json = nlohmann::ordered_json;

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

bool WriteJsonAA(const ordered_json& data, const wstring& filename) noexcept
{
	ofstream file(GetResourcePath() + filename);
	if (!file.is_open())
		return false;

	file << data.dump(4);
	file.close();

	return true;
}

json DataToJson(const wstring& data) noexcept
{
	return RemoveNullTerminator(WStringToString(data));
}

void DataToJson(const string& key, const string& data, ordered_json& outJson) noexcept
{
	outJson[key] = data;
}

void DataToJson(const string& key, const wstring& data, ordered_json& outJson) noexcept
{
	outJson[key] = DataToJson(data);
}

void DataToJson(const string& key, const map<wstring, size_t>& data, ordered_json& outJson) noexcept
{
	json j{};
	for (const auto& font : data)
		j[DataToJson(font.first)] = font.second;

	outJson[key] = j;
}

void DataToJson(const string& key, const map<wstring, wstring>& data, ordered_json& outJson) noexcept
{
	json j{};
	for (const auto& font : data)
		j[DataToJson(font.first)] = DataToJson(font.second);

	outJson[key] = j;
}

void DataToJson(const string& key, const Rectangle& rect, ordered_json& outJson) noexcept
{
	ordered_json j{};

	j["x"] = rect.x;
	j["y"] = rect.y;
	j["width"] = rect.width;
	j["height"] = rect.height;

	outJson[key] = j;
}

void DataToJson(const string& key, Origin origin, ordered_json& outJson) noexcept
{
	outJson[key] = EnumToString(origin);
}

void DataFromJson(const string& key, string& outStr, const json& j) noexcept
{
	outStr = j[key];
}

void DataFromJson(const string& key, Rectangle& outRect, const json& j) noexcept
{
	const auto& keyJ = j[key];
	outRect.x = keyJ["x"];
	outRect.y = keyJ["y"];
	outRect.width = keyJ["width"];
	outRect.height = keyJ["height"];
}

void DataFromJson(const string& key, Origin& outOrigin, const json& j) noexcept
{
	outOrigin = StringToEnum<Origin>(j[key]);
}

void DataFromJson(const string& key, map<wstring, wstring>& outData, const json& j) noexcept
{
	const auto& keyJ = j[key];
	for(const auto& [k, v] : keyJ.items())
		outData.insert(make_pair(StringToWString(k), StringToWString(v)));
}

void DataFromJson(const string& key, wstring& outData, const json& j) noexcept
{
	outData = StringToWString(j[key]);
}

json ReadJsonAA(const wstring& filename) noexcept
{
	ifstream file(GetResourcePath() + filename);
	if (!file.is_open())
		return nullptr;

	return json::parse(file);
}