#pragma once
#include "ComponentT.h"
#include "Toy/UserInterface/UIComponent/UIUtility.h"
#include "Toy/UserInterface/UIComponent/Traverser/UITraverser.h"
#include "Toy/UserInterface/UIComponent/Components/TextureSwitcher.h"

//마우스를 사용해 상태변경은 UIModule에서 하기 때문에 관련 event 및 callback 테스트는 UIModule에서.
class TextureSwitcherT : public ComponentT
{
protected:
	virtual void SetUp() override;
	virtual void TearDown() override {};
	virtual void RegisterBinderTextures(MockTextureResourceBinder* resBinder) override;

	unique_ptr<TextureSwitcher> m_component;
};

void TextureSwitcherT::SetUp()
{
	ComponentT::SetUp();

	m_component = CreateComponent<TextureSwitcher>(TextureSlice::One, GetStateKeyMap("Button32"));
	UITraverser::BindTextureSourceInfo(m_component.get(), GetResBinder(), GetTextureController());
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