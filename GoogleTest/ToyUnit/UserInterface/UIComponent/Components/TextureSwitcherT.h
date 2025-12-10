#pragma once
#include "UserInterface/UIModuleT.h"
#include "Toy/UserInterface/UIComponent/UIUtility.h"
#include "Toy/UserInterface/UIComponent/Components/TextureSwitcher.h"

//마우스를 사용해 상태변경은 UIModule에서 하기 때문에 관련 event 및 callback 테스트는 UIModule에서.
class TextureSwitcherT : public UIModuleT
{
protected:
	virtual void SetUp() override;
	virtual void RegisterBinderTextures(MockTextureResourceBinder* resBinder) override;

	TextureSwitcher* m_component{ nullptr };
};

void TextureSwitcherT::SetUp()
{
	UIModuleT::SetUp();

	auto component = CreateComponent<TextureSwitcher>(TextureSlice::One, GetStateKeyMap("Button32"));
	m_component = component.get();
	AttachComponent(m_main, move(component));
	m_uiModule->BindTextureResources();
}

void TextureSwitcherT::RegisterBinderTextures(MockTextureResourceBinder* resBinder)
{
	resBinder->AddMockTextureKeys({
		{ "Button32_Normal", { L"Texture512.png", TextureSlice::One, { { 0, 0, 30, 30 } } } },
		{ "Button32_Hovered", { L"Texture512.png", TextureSlice::One, { { 0, 0, 40, 40 } } } },
		{ "Button32_Pressed", { L"Texture512.png", TextureSlice::One, { { 0, 0, 50, 50 } } } },
		{ "Button64_Normal", { L"Texture512.png", TextureSlice::One, { { 0, 0, 60, 60 } } } },
		});
}