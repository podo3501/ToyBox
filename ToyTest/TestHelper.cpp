#include "pch.h"
#include "../Include/IComponent.h"
#include "../Toy/Config.h"
#include "../Toy/UserInterface/Scene.h"
#include "../Toy/UserInterface/UIComponent.h"
#include "../Toy/UserInterface/JsonHelper.h"
#include "IMockRenderer.h"

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

bool WriteReadTest(unique_ptr<UIComponent>& write, const wstring& filename = L"UI/Data/WriteReadTest.json")
{
	WriteJsonFile(write, L"UI/Data/JOPTest.json");

	unique_ptr<UIComponent> read;
	ReadJsonFile(L"UI/Data/JOPTest.json", read);

	EXPECT_TRUE(*write == *read);

	return true;
}

void CallMockRender(IComponent* component, function<void(size_t, const RECT&, const RECT*)> testRenderFunc)
{
	using ::testing::_;
	using ::testing::Invoke;

	MockRender mockRender;
	EXPECT_CALL(mockRender, Render(_, _, _)).WillRepeatedly(Invoke(testRenderFunc));
	component->Render(&mockRender);
}