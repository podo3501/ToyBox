#include "pch.h"
#include "../Toy/Config.h"
#include "../Toy/UserInterface/Scene.h"
#include "../Toy/UserInterface/JsonHelper.h"

using json = nlohmann::json;

bool WriteReadTest(Scene& sourceScene, const wstring& filename = L"UI/Data/WriteReadTest.json")
{
	WriteJsonFile(sourceScene, L"UI/Data/JOPTest.json");

	Scene destReadOpScene(GetRectResolution());
	ReadJsonFile(L"UI/Data/JOPTest.json", destReadOpScene);

	EXPECT_TRUE(sourceScene == destReadOpScene);
	WriteJsonFile(sourceScene, L"UI/Data/JOPTest2.json");

	return true;
}