#pragma once
#include "ComponentT.h"
#include "Toy/UserInterface/UIComponent/UIUtility.h"
#include "Toy/UserInterface/UIComponent/Traverser/UITraverser.h"
#include "Toy/UserInterface/UIComponent/Components/ScrollBar.h"
#include "Toy/UserInterface/UIComponent/Components/PatchTexture/PatchTextureStd/PatchTextureStd3.h"
#include "Toy/UserInterface/UIComponent/Components/TextureSwitcher.h"
#include "Toy/UserInterface/UIComponent/Components/SampleComponent.h"

class ScrollBarT : public ComponentT
{
protected:
	virtual void SetUp() override;
	virtual void TearDown() override {};
	virtual void RegisterBinderTextures(MockTextureResourceBinder* resBinder) override;

	unique_ptr<ScrollBar> m_component;
	TextureSwitcher* m_button{ nullptr };
};

void ScrollBarT::SetUp()
{
	ComponentT::SetUp();

	UILayout layout{};
	DirectionType dirType{ DirectionType::Vertical };

	m_component = CreateSampleScrollBar({}, DirectionType::Vertical, "Track", "Button", &m_button);
	UITraverser::BindTextureSourceInfo(m_component.get(), GetResBinder(), GetTextureController());

	m_component->UpdateScrollView(50, 200); //보여줄 데이터가 화면영역보다 많아야 스크롤바가 생긴다.
	m_component->SetPositionRatio(0.5f); //중간으로 버튼을 이동힌다.
	m_component->UpdatePositionsManually();
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