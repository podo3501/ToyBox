#pragma once
#include "UserInterface/UIComponent/UIComponent.h"
#include "UserInterface/UIModule.h"

struct ITextureLoad;
class TextureResourceBinder;

//?!?이 함수는 사라질 예정
unique_ptr<UIComponent> WriteReadTest(unique_ptr<UIComponent>& write, const wstring& filename = L"Test/Data/RWUserInterfaceTest.json");

//?!?이 함수는 사라질 예정
template<typename T>
unique_ptr<UIComponent> WriteReadTest(unique_ptr<UIComponent>& write, T& component, const wstring& filename = L"Test/Data/RWUserInterfaceTest.json")
{
	auto read = WriteReadTest(write, filename);
	component = UIEx(read).FindComponent<T>(component->GetName());

	return move(read);
}

unique_ptr<UIModule> WriteReadTest(IRenderer* renderer, unique_ptr<UIModule>& write, const wstring& filename = L"Test/Data/RWUserInterfaceTest.json");
template<typename T>
pair<unique_ptr<UIModule>, UIComponent*> WriteReadTest(IRenderer* renderer, unique_ptr<UIModule>& write, T& component, const wstring& filename = L"Test/Data/RWUserInterfaceTest.json")
{
	auto read = WriteReadTest(renderer, write, filename);
	component = UIEx(read->GetMainPanel()).FindComponent<T>(component->GetName());

	return { move(read), read->GetMainPanel() };
}

unique_ptr<UIComponent> WriteReadTest(TextureResourceBinder* binder,
	unique_ptr<UIComponent>& write, const wstring& filename = L"Test/Data/RWUserInterfaceTest.json");

template<typename T>
unique_ptr<UIComponent> WriteReadTest(TextureResourceBinder* binder, 
	unique_ptr<UIComponent>& write, T& component, const wstring& filename = L"Test/Data/RWUserInterfaceTest.json")
{
	auto read = WriteReadTest(binder, write, filename);
	component = UIEx(read).FindComponent<T>(component->GetName());

	return move(read);
}

bool TestSourceBinderWriteRead(TextureResourceBinder* resBinder, const wstring& filename = L"Test/Data/RWSourceBinderTest.json");

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
	size_t expIndex, const vector<RECT>& expDests, const vector<RECT>& expSrcs) noexcept;
vector<RECT> GetSources(TextureResourceBinder* resBinder, const string& key) noexcept;
