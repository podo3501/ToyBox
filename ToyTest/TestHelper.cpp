#include "pch.h"
#include "../Toy/UserInterface/Scene.h"
#include "../Toy/UserInterface/JsonHelper.h"
#include "../Toy/UserInterface/JsonOperation.h"

using json = nlohmann::json;

bool WriteReadTest(Scene& sourceScene, const wstring& filename = L"UI/Data/WriteReadTest.json")
{
	unique_ptr<JsonOperation> jsonWriteOp = make_unique<JsonOperation>();
	sourceScene.SerializeIO(jsonWriteOp.get());
	jsonWriteOp->Write(L"UI/Data/JOPTest.json");

	unique_ptr<JsonOperation> jsonReadOp = make_unique<JsonOperation>();
	jsonReadOp->Read(L"UI/Data/JOPTest.json");
	Scene destReadOpScene;
	destReadOpScene.SerializeIO(jsonReadOp.get());

	EXPECT_TRUE(sourceScene == destReadOpScene);
	unique_ptr<JsonOperation> newOp = make_unique<JsonOperation>();
	destReadOpScene.SerializeIO(newOp.get());
	newOp->Write(L"UI/Data/JOPTest2.json");
	
	//이걸로 만들어야 하나 고민중
	//JsonOperation jsonOp(L"UI/Data/WriteReadTest.json", true);
	//sourceScene->Process(jsonOp);

	//JsonOperation jsonOp(L"UI/Data/WriteReadTest.json", false);
	//Scene destScene;
	//destScene->Process(jsonOp);

	//return sourceScene == destScene;

	nlohmann::ordered_json j = sourceScene;
	WriteJson(j, filename);

	json readJson = ReadJson(filename);
	Scene destScene;
	destScene = readJson.get<Scene>();

	return sourceScene == destScene;
}