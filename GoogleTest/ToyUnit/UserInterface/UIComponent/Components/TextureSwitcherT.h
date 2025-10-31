#pragma once
#include "ComponentT.h"
#include "Toy/UserInterface/UIComponent/UIUtility.h"
#include "Toy/UserInterface/UIComponent/Components/TextureSwitcher.h"

//���콺�� ����� ���º����� UIModule���� �ϱ� ������ ���� event �� callback �׽�Ʈ�� UIModule����.
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

	m_component = CreateComponent<TextureSwitcher>(TextureSlice::One,
		GetStateKeyMap("Button32"), BehaviorMode::Normal);
	m_component->BindTextureSourceInfo(GetResBinder(), GetTextureController());
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