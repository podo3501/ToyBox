#pragma once

struct IComponent;
class UIComponent;

bool WriteReadTest(unique_ptr<UIComponent>& write, const wstring& filename = L"UI/Data/WriteReadTest.json");
void CallMockRender(IComponent* component, function<void(size_t, const RECT&, const RECT*, bool)> testRenderFunc, int times);
void CallMockRender(IComponent* component, function<void(size_t, const wstring&, const Vector2&, const FXMVECTOR&)> testRenderFunc);
void TestUpdate(HWND hwnd, UIComponent* component, int mouseX, int mouseY);
