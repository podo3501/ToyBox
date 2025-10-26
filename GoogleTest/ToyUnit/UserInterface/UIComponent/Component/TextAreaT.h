#pragma once
#include "ComponentT.h"
#include "Toy/UserInterface/UIComponent/Components/TextArea.h"

class TextAreaT : public ComponentT
{
protected:
	virtual void SetUp() override;
	virtual void TearDown() override {};
	virtual void RegisterRenderTextures(MockRenderer* renderer) override;
	virtual void RegisterBinderTextures(MockTextureResourceBinder* resBinder) override;
	
	unique_ptr<TextArea> m_component;
};

void TextAreaT::SetUp()
{
	ComponentT::SetUp();

	vector<wstring> bindKeys{ L"Hangle", L"English" };
	wstring text = L"<Hangle>�׽�Ʈ<Red>������</Red></Hangle><br><English>Test<Blue>Blue</Blue></English>";
	m_component = CreateComponent<TextArea>(text, bindKeys);
	m_component->BindTextureSourceInfo(GetResBinder(), GetTextureController());
}

void TextAreaT::RegisterRenderTextures(MockRenderer* renderer)
{
	renderer->RegisterMockTextureInfos({
		{ L"MockHangleFont.spritefont", 0, {} },
		{ L"MockEnglishFont.spritefont", 1, {} },
		});
}

void TextAreaT::RegisterBinderTextures(MockTextureResourceBinder* resBinder)
{
	resBinder->AddMockFontKeys({
		{ L"Hangle", TextureFontInfo{ L"MockHangleFont.spritefont" } },
		{ L"English", TextureFontInfo{ L"MockEnglishFont.spritefont" } },
		});
}