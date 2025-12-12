#pragma once
#include "UserInterface/UIComponent/ComponentFixture.h"
#include "Toy/UserInterface/UIComponent/Components/PatchTexture/PatchTextureStd/PatchTextureStd1.h"

class PatchTextureStd1T : public ComponentFixture
{
protected:
	virtual void SetUp() override;
	virtual void RegisterBinderTextures(MockTextureResourceBinder* resBinder) override;

	unique_ptr<PatchTextureStd1> m_component;
};

///////////////////////////////////////////////////////////////

void PatchTextureStd1T::SetUp()
{
	ComponentFixture::SetUp();

	m_component = CreateComponent<PatchTextureStd1>("One32");
	BindTextureSourceInfo(m_component.get(), GetResBinder());
}

void PatchTextureStd1T::RegisterBinderTextures(MockTextureResourceBinder* resBinder)
{
	resBinder->AddMockTextureKeys({
		{ "One32", { L"Texture512.png", TextureSlice::One, { { 0, 0, 32, 32 } } } },
		{ "One64", { L"Texture512.png", TextureSlice::One, { { 0, 0, 64, 64 } } } },
		});
}