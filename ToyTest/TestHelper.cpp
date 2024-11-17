#include "pch.h"
#include "../Toy/UserInterface/Scene.h"
#include "../Toy/UserInterface/JsonHelper.h"

using json = nlohmann::json;

bool WriteReadTest(const Scene& sourceScene, const wstring& filename = L"UI/Data/WriteReadTest.json")
{
	nlohmann::ordered_json j = sourceScene;
	WriteJson(j, filename);

	json readJson = ReadJson(filename);
	Scene destScene;
	destScene = readJson.get<Scene>();

	return sourceScene == destScene;
}