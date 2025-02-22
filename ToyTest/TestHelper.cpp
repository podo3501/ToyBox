#include "pch.h"
#include "TestHelper.h"
#include "../Include/IComponent.h"
#include "../Toy/Config.h"
#include "../Toy/UserInterface/UIComponent.h"
#include "../Toy/UserInterface/JsonHelper.h"

using json = nlohmann::json;

bool WriteReadTest(unique_ptr<UIComponent>& write, const wstring& filename)
{
	JsonFile::WriteComponent(write, L"Test/Data/JOPTest.json");

	unique_ptr<UIComponent> read;
	JsonFile::ReadComponent(L"Test/Data/JOPTest.json", read);

	EXPECT_TRUE(*write == *read);

	return true;
}

unique_ptr<UIComponent> TestComponent::CreateClone() const { return nullptr; }
bool TestComponent::ImplementLoadResource(ITextureLoad* load) { return m_loadTestFunc(load); };
void TestComponent::SetLoadTestFunction(function<bool(ITextureLoad*)> func) { m_loadTestFunc = move(func); }
