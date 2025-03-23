#pragma once
#include "../Toy/UserInterface/UIComponent.h"

struct ITextureLoad;
class TextureSourceBinder;

bool WriteReadTest(unique_ptr<UIComponent>& write, const wstring& filename = L"Test/Data/RWUserInterfaceTest.json");
bool WriteReadTest(unique_ptr<TextureSourceBinder>& sourceBinder, const wstring& filename = L"Test/Data/RWSourceBinderTest.json");

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

void TestCoordinates(size_t index, const RECT& dest, const RECT* source,
	const vector<RECT>& expDests, const vector<RECT>& expSrcs) noexcept;
vector<RECT> GetSources(TextureSourceBinder* sourceBinder, const string& key) noexcept;
