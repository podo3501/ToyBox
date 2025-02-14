#pragma once
#include "../Toy/UserInterface/UIComponent.h"
struct IComponent;
struct ILoadData;

bool WriteReadTest(unique_ptr<UIComponent>& write, const wstring& filename = L"UI/Data/WriteReadTest.json");
void CallMockRender(IComponent* component, function<void(size_t, const RECT&, const RECT*)> testRenderFunc, int times);
void CallMockRender(IComponent* component, function<void(size_t, const wstring&, const Vector2&, const FXMVECTOR&)> testRenderFunc);
void TestUpdate(UIComponent* component, int mouseX, int mouseY);

class TestComponent : public UIComponent
{
public:
	static ComponentID GetTypeStatic() { return ComponentID::Unknown; }
	virtual ComponentID GetTypeID() const noexcept override { return GetTypeStatic(); }
	virtual bool LoadResources(ILoadData* load) override;

	void SetLoadTestFunction(function<void(ILoadData*)> func);

protected:
	virtual unique_ptr<UIComponent> CreateClone() const override;

private:
	function<void(ILoadData*)> m_loadTestFunc;
};