#pragma once

class Scene;
struct IComponent;
class UIComponent;

bool WriteReadTest(Scene& sourceScene, const wstring& filename = L"UI/Data/WriteReadTest.json");

bool WriteReadTest(unique_ptr<UIComponent>& write, const wstring& filename = L"UI/Data/WriteReadTest.json");

void CallMockRender(IComponent* component, function<void(size_t, const RECT&, const RECT*)> testRenderFunc);
