#pragma once
#include "UserInterface/UIComponent/Components/ComponentT.h"
#include "Toy/UserInterface/UIComponent/Traverser/UITraverser.h"
#include "Toy/UserInterface/UIComponent/Components/PatchTexture/PatchTextureStd/PatchTextureStd1.h"

class PatchTextureStd1T : public ComponentT
{
protected:
	virtual void SetUp() override;
	virtual void TearDown() override {};
	virtual void RegisterBinderTextures(MockTextureResourceBinder* resBinder) override;

	unique_ptr<PatchTextureStd1> m_component;
};

///////////////////////////////////////////////////////////////

void PatchTextureStd1T::SetUp()
{
	ComponentT::SetUp();

	m_component = CreateComponent<PatchTextureStd1>("One32");
	UITraverser::BindTextureSourceInfo(m_component.get(), GetResBinder(), GetTextureController());
}

void PatchTextureStd1T::RegisterBinderTextures(MockTextureResourceBinder* resBinder)
{
	resBinder->AddMockTextureKeys({
		{ "One32", { L"Texture512.png", TextureSlice::One, { { 0, 0, 32, 32 } } } },
		{ "One64", { L"Texture512.png", TextureSlice::One, { { 0, 0, 64, 64 } } } },
		});
}