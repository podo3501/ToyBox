#include "pch.h"
#include "UIType.h"
#include "../Utility.h"

using json = nlohmann::json;

bool ImageSource::Read(const json& data)
{
	filename = StringToWString(data["Filename"]);

	if(!data.contains("Position")) return true;

	auto& posList = data["Position"];
	for (size_t i{ 0 }; posList.size() != i; i++)
		list.emplace_back(posList[i][0], posList[i][1], posList[i][2], posList[i][3]);

	return true;
}