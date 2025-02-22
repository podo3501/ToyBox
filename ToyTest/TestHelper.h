#pragma once
#include "../Toy/UserInterface/UIComponent.h"
struct ITextureLoad;

bool WriteReadTest(unique_ptr<UIComponent>& write, const wstring& filename = L"UI/Data/WriteReadTest.json");

class TestComponent : public UIComponent
{
public:
	static ComponentID GetTypeStatic() { return ComponentID::Unknown; }
	virtual ComponentID GetTypeID() const noexcept override { return GetTypeStatic(); }
	void SetLoadTestFunction(function<bool(ITextureLoad*)> func);

protected:
	virtual unique_ptr<UIComponent> CreateClone() const override;
	virtual bool ImplementLoadResource(ITextureLoad* load) override;

private:
	function<bool(ITextureLoad*)> m_loadTestFunc;
};