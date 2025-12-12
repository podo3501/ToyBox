#pragma once
#include "../ComponentFixture.h"
#include "Toy/UserInterface/UIComponent/Components/TextArea.h"
#include "Toy/UserInterface/UIComponentLocator.h"

class TextAreaT : public ComponentFixture
{
protected:
	virtual void SetUp() override;
	virtual void RegisterRenderTextures(MockRenderer* renderer) override;
	virtual void RegisterBinderTextures(MockTextureResourceBinder* resBinder) override;
	
	unique_ptr<UIComponentManager> m_componentManager;
	unique_ptr<TextArea> m_component;
};

void TextAreaT::SetUp()
{
	ComponentFixture::SetUp();

	m_componentManager = make_unique<UIComponentManager>(GetRenderer());
	UIComponentLocator::Provide(m_componentManager.get());

	vector<wstring> bindKeys{ L"Hangle", L"English" };
	wstring text = L"<Hangle>테스트<Red>빨강색</Red></Hangle><br><English>Test<Blue>Blue</Blue></English>";
	m_component = CreateComponent<TextArea>(GetRenderer()->GetTextureController(), text, bindKeys);
	BindTextureSourceInfo(m_component.get(), GetResBinder());
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