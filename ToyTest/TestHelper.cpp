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
	JsonFile::WriteComponent(write, L"UI/Data/JOPTest.json");

	unique_ptr<UIComponent> read;
	JsonFile::ReadComponent(L"UI/Data/JOPTest.json", read);

	EXPECT_TRUE(*write == *read);

	return true;
}

using ::testing::_;
using ::testing::Invoke;

void CallMockRender(IComponent* component, function<void(size_t, const RECT&, const RECT*)> testRenderFunc, int times)
{
	MockRender mockRender;
	EXPECT_CALL(mockRender, Render(_, _, _))
		.Times(times)
		.WillRepeatedly(Invoke(testRenderFunc));
	component->ProcessRender(&mockRender);
}

void CallMockRender(IComponent* component, function<void(size_t, const wstring&, const Vector2&, const FXMVECTOR&)> testRenderFunc)
{
	MockRender mockRender;
	EXPECT_CALL(mockRender, DrawString(_, _, _, _)).WillRepeatedly(Invoke(testRenderFunc));
	component->ProcessRender(&mockRender);
}

void TestUpdate(HWND hwnd, UIComponent* component, int mouseX, int mouseY)
{
	InputManager inputManager(hwnd);
	Mouse::State mouseState;
	mouseState.x = mouseX;
	mouseState.y = mouseY;
	inputManager.Update(mouseState);

	component->ProcessUpdate({}, &inputManager);
}