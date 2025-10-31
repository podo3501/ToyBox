#pragma once
#include "UserInterface/UIComponent/Components/ComponentT.h"
#include "Toy/UserInterface/UIComponent/Components/PatchTexture/PatchTextureStd/PatchTextureStd3.h"

class PatchTextureStd3T_Horizontal : public ComponentT
{
protected:
	virtual void SetUp() override;
	virtual void TearDown() override {};
	virtual void RegisterBinderTextures(MockTextureResourceBinder* resBinder) override;

	unique_ptr<PatchTextureStd3> m_component;
};

///////////////////////////////////////////////////////////////

void PatchTextureStd3T_Horizontal::SetUp()
{
	ComponentT::SetUp();

	m_component = CreateComponent<PatchTextureStd3>(DirectionType::Horizontal, "ThreeH48");
	m_component->BindTextureSourceInfo(GetResBinder(), GetTextureController());
}

void PatchTextureStd3T_Horizontal::RegisterBinderTextures(MockTextureResourceBinder* resBinder)
{
	resBinder->AddMockTextureKeys({
		{ "ThreeH48", { L"Texture512.png", TextureSlice::ThreeH, { { 0, 0, 22, 48 }, { 22, 0, 4, 48 }, { 26, 0, 22, 48 } } } },
		{ "ThreeH60", { L"Texture512.png", TextureSlice::ThreeH, { { 0, 0, 22, 60 }, { 22, 0, 16, 60 }, { 38, 0, 22, 60 } } } }
		});
}