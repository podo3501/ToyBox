#pragma once
#include "ComponentT.h"
#include "Toy/UserInterface/UIComponent/UIUtility.h"
#include "Toy/UserInterface/UIComponent/Components/ScrollBar.h"
#include "Toy/UserInterface/UIComponent/Components/PatchTexture/PatchTextureStd/PatchTextureStd3.h"
#include "Toy/UserInterface/UIComponent/Components/TextureSwitcher.h"

class ScrollBarT : public ComponentT
{
protected:
	virtual void SetUp() override;
	virtual void TearDown() override {};
	virtual void RegisterBinderTextures(MockTextureResourceBinder* resBinder) override;

	unique_ptr<ScrollBar> m_component;
};

void ScrollBarT::SetUp()
{
	ComponentT::SetUp();

	//Track과 Button으로 이루어져 있다.
	m_component = CreateComponent<ScrollBar>(UILayout{},
		CreateComponent<PatchTextureStd3>(UILayout{}, DirectionType::Vertical, "Track"),
		CreateComponent<TextureSwitcher>(UILayout{}, DirTypeToTextureSlice(DirectionType::Vertical), 
			GetStateKeyMap("Button"), BehaviorMode::HoldToKeepPressed));
	m_component->BindTextureSourceInfo(GetResBinder(), GetTextureController());
}

void ScrollBarT::RegisterBinderTextures(MockTextureResourceBinder* resBinder)
{
	resBinder->AddMockTextureKeys({
		{ "Track", { L"Texture512.png", TextureSlice::ThreeV, { { 0, 20, 20, 5 }, { 0, 25, 20, 20 }, { 0, 45, 20, 5 } } } }, //폭 20, 높이 30
		{ "Button_Normal", { L"Texture512.png", TextureSlice::ThreeV, { { 0, 0, 20, 4 }, { 0, 4, 20, 12 }, { 0, 16, 20, 4 } } } }, //폭 20, 높이 20
		{ "Button_Hovered", { L"Texture512.png", TextureSlice::ThreeV, { { 0, 0, 20, 4 }, { 0, 4, 20, 12 }, { 0, 16, 20, 4 } } } },
		{ "Button_Pressed", { L"Texture512.png", TextureSlice::ThreeV, { { 0, 0, 20, 4 }, { 0, 4, 20, 12 }, { 0, 16, 20, 4 } } } }
		});
}