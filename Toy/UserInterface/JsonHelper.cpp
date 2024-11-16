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
#include "TransformComponent.h"

using json = nlohmann::json;
using ordered_json = nlohmann::ordered_json;

double RoundToSix(double value) 
{
	return std::round(value * 1e6) / 1e6;
}

bool WriteJson(ordered_json& data, const wstring& filename) noexcept
{
	ofstream file(GetResourcePath() + filename);
	if (!file.is_open())
		return false;

	file << data.dump(4);
	file.close();

	return true;
}

json ReadJson(const wstring& filename) noexcept
{
	ifstream file(GetResourcePath() + filename);
	if (!file.is_open())
		return nullptr;

	return json::parse(file);
}

json DataToJson(const wstring& data) noexcept
{
	return RemoveNullTerminator(WStringToString(data));
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

void DataToJson(const string& key, const Vector2& vec, ordered_json& outJson) noexcept
{
	ordered_json j{};

	j["x"] = RoundToSix(vec.x);
	j["y"] = RoundToSix(vec.y);

	outJson[key] = j;
}

void DataToJson(const string& key, const vector<unique_ptr<TransformComponent>>& data, ordered_json& outJson) noexcept
{
	if (data.empty())
		return;
	
	outJson[key] = json::array();
	for (const auto& prop : data)
		outJson[key].push_back(*prop);
}

/////////////////////////////////////////////////////////////////////////////////////

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

void DataFromJson(const string& key, Vector2& outData, const json& j) noexcept
{
	const auto& keyJ = j[key];
	outData.x = keyJ["x"];
	outData.y = keyJ["y"];
}

void DataFromJson(const string& key, vector<unique_ptr<TransformComponent>>& outData, const json& j) noexcept
{
	if (j.contains(key) == false)
		return;

	outData.clear();
	for (const auto& propJson : j[key])
	{
		auto prop = std::make_unique<TransformComponent>(propJson.get<TransformComponent>());
		outData.emplace_back(move(prop));
	}
}