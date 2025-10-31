#pragma once
#include "ComponentT.h"
#include "Toy/UserInterface/UIComponent/UIUtility.h"
#include "Toy/UserInterface/UIComponent/Components/ScrollBar.h"
#include "Toy/UserInterface/UIComponent/Components/SampleComponent.h"

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

	//PatchTextureStd와 TextureSwitcher로 이루어져 있다.
	m_component = CreateSampleScrollBar({}, DirectionType::Vertical, "Track", "Button");
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