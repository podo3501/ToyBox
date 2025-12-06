#pragma once
#include "UserInterface/UIComponent/Components/ComponentT.h"
#include "Toy/UserInterface/UIComponent/Components/RenderTexture.h"
#include "Toy/UserInterface/UIComponent/Components/PatchTexture/PatchTextureStd/PatchTextureStd1.h"

class RenderTextureT : public ComponentT
{
protected:
	virtual void SetUp() override;
	virtual void RegisterBinderTextures(MockTextureResourceBinder* resBinder) override;

	unique_ptr<RenderTexture> m_component;
};

void RenderTextureT::SetUp()
{
	ComponentT::SetUp();

	auto tex = CreateComponent<PatchTextureStd1>("One32");
	m_component = CreateComponent<RenderTexture>(move(tex));
	BindTextureSourceInfo(m_component.get(), GetResBinder());
}

void RenderTextureT::RegisterBinderTextures(MockTextureResourceBinder* resBinder)
{
	resBinder->AddMockTextureKeys({
		{ "One32", { L"Texture512.png", TextureSlice::One, { { 0, 0, 32, 32 } } } }
		});
}