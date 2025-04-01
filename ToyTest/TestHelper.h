#pragma once
#include "../Toy/UserInterface/UIComponent/UIComponent.h"

struct ITextureLoad;
class TextureSourceBinder;

unique_ptr<UIComponent> WriteReadTest(unique_ptr<UIComponent>& write, const wstring& filename = L"Test/Data/RWUserInterfaceTest.json");

template<typename T>
unique_ptr<UIComponent> WriteReadTest(unique_ptr<UIComponent>& write, T& component, const wstring& filename = L"Test/Data/RWUserInterfaceTest.json")
{
	auto read = WriteReadTest(write, filename);
	component = UIEx(read).FindComponent<T>(component->GetName());

	return move(read);
}

bool TestSourceBinderWriteRead(unique_ptr<TextureSourceBinder>& sourceBinder, const wstring& filename = L"Test/Data/RWSourceBinderTest.json");

class TestComponent : public UIComponent
{
public:
	static ComponentID GetTypeStatic() { return ComponentID::Unknown; }
	virtual ComponentID GetTypeID() const noexcept override { return GetTypeStatic(); }
	void SetLoadTestFunction(function<bool(ITextureLoad*)> func);

protected:
	virtual unique_ptr<UIComponent> CreateClone() const override;

private:
	function<bool(ITextureLoad*)> m_loadTestFunc;
};

void TestCoordinates(size_t index, const RECT& dest, const RECT* source,
	const vector<RECT>& expDests, const vector<RECT>& expSrcs) noexcept;
vector<RECT> GetSources(TextureSourceBinder* sourceBinder, const string& key) noexcept;
