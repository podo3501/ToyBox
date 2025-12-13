#pragma once
#include "UserInterface/UIComponent/ComponentFixture.h"
#include "Toy/UserInterface/UIComponent/Components/PatchTexture/PatchTextureStd/PatchTextureStd3.h"

class PatchTextureStd3T_Vertical : public ComponentFixture
{
protected:
	virtual void SetUp() override;
	virtual void RegisterBinderTextures(MockTextureResourceBinder* resBinder) override;

	unique_ptr<PatchTextureStd3> m_component;
};

///////////////////////////////////////////////////////////////

void PatchTextureStd3T_Vertical::SetUp()
{
	ComponentFixture::SetUp();

	m_component = CreateComponent<PatchTextureStd3>(DirectionType::Vertical, "ThreeV48");
	BindResource(m_component.get());
}

void PatchTextureStd3T_Vertical::RegisterBinderTextures(MockTextureResourceBinder* resBinder)
{
	resBinder->AddMockTextureKeys({
		{ "ThreeV48", { L"Texture512.png", TextureSlice::ThreeV, { { 0, 0, 48, 22 }, { 0, 22, 48, 4 }, { 0, 26, 48, 22 } } } },
		{ "ThreeV60", { L"Texture512.png", TextureSlice::ThreeV, { { 0, 0, 60, 22 }, { 0, 22, 60, 16 }, { 0, 38, 60, 22 } } } }
		});
}