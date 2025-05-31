#pragma once
#include "../Toy/UserInterface/UIComponent/UIComponent.h"
#include "../Toy/UserInterface/UIModule.h"

struct ITextureLoad;
class TextureResourceBinder;

//?!?�� �Լ��� ����� ����
unique_ptr<UIComponent> WriteReadTest(unique_ptr<UIComponent>& write, const wstring& filename = L"Test/Data/RWUserInterfaceTest.json");

//?!?�� �Լ��� ����� ����
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
	component = UIEx(read->GetComponent()).FindComponent<T>(component->GetName());

	return { move(read), read->GetComponent() };
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

bool TestSourceBinderWriteRead(unique_ptr<TextureResourceBinder>& resBinder, const wstring& filename = L"Test/Data/RWSourceBinderTest.json");

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
