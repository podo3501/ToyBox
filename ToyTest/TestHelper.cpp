#include "pch.h"
#include "../Include/IComponent.h"
#include "../Toy/Config.h"
#include "../Toy/InputManager.h"
#include "../Toy/UserInterface/UIComponent.h"
#include "../Toy/UserInterface/JsonHelper.h"
#include "IMockRenderer.h"

using json = nlohmann::json;

bool WriteReadTest(unique_ptr<UIComponent>& write, const wstring& filename = L"UI/Data/WriteReadTest.json")
{
	WriteJsonFile(write, L"UI/Data/JOPTest.json");

	unique_ptr<UIComponent> read;
	ReadJsonFile(L"UI/Data/JOPTest.json", read);

	EXPECT_TRUE(*write == *read);

	return true;
}

void CallMockRender(IComponent* component, function<void(size_t, const RECT&, const RECT*, bool)> testRenderFunc)
{
	using ::testing::_;
	using ::testing::Invoke;

	MockRender mockRender;
	EXPECT_CALL(mockRender, Render(_, _, _, _)).WillRepeatedly(Invoke(testRenderFunc));
	component->Render(&mockRender);
}

void TestUpdate(HWND hwnd, UIComponent* component, int mouseX, int mouseY)
{
	InputManager inputManager(hwnd);
	Mouse::State mouseState;
	mouseState.x = mouseX;
	mouseState.y = mouseY;
	inputManager.Update(mouseState);

	component->Update({}, &inputManager);
}