#pragma once
#include "ComponentT.h"
#include "Toy/UserInterface/UIComponent/Components/Container.h"
#include "Toy/UserInterface/UIComponent/Components/PatchTexture/PatchTextureStd/PatchTextureStd1.h"

class ContainerT : public ComponentT
{
protected:
	virtual void SetUp() override;
	virtual void RegisterBinderTextures(MockTextureResourceBinder* resBinder) override;

	unique_ptr<Container> m_component;
};

void ContainerT::SetUp()
{
	ComponentT::SetUp();

	map<InteractState, unique_ptr<UIComponent>> componentList;
	componentList[InteractState::Normal] = CreateComponent<PatchTextureStd1>("Button32_Normal");
	componentList[InteractState::Hovered] = CreateComponent<PatchTextureStd1>("Button32_Hovered");
	componentList[InteractState::Pressed] = CreateComponent<PatchTextureStd1>("Button32_Pressed");

	m_component = CreateComponent<Container>(UILayout{}, move(componentList));
	BindTextureSourceInfo(m_component.get(), GetResBinder());
}

void ContainerT::RegisterBinderTextures(MockTextureResourceBinder* resBinder)
{
	resBinder->AddMockTextureKeys({
		{ "Button32_Normal", { L"Texture512.png", TextureSlice::One, { { 0, 0, 30, 30 } } } },
		{ "Button32_Hovered", { L"Texture512.png", TextureSlice::One, { { 0, 0, 40, 40 } } } },
		{ "Button32_Pressed", { L"Texture512.png", TextureSlice::One, { { 0, 0, 50, 50 } } } },
		});
}