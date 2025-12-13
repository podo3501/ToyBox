#pragma once
#include "../ComponentFixture.h"
#include "Toy/UserInterface/UIComponent/Components/RenderTexture.h"
#include "Toy/UserInterface/UIComponent/Components/PatchTexture/PatchTextureStd/PatchTextureStd1.h"
#include "Toy/UserInterface/UIComponentLocator.h"

class RenderTextureT : public ComponentFixture
{
protected:
	virtual void SetUp() override;
	virtual void RegisterBinderTextures(MockTextureResourceBinder* resBinder) override;

	unique_ptr<UIComponentManager> m_componentManager;
	unique_ptr<RenderTexture> m_component;
};

void RenderTextureT::SetUp()
{
	ComponentFixture::SetUp();

	m_componentManager = make_unique<UIComponentManager>(GetRenderer());
	UIComponentLocator::Provide(m_componentManager.get());

	auto tex = CreateComponent<PatchTextureStd1>("One32");
	m_component = CreateComponent<RenderTexture>(move(tex));
	BindResource(m_component.get());
}

void RenderTextureT::RegisterBinderTextures(MockTextureResourceBinder* resBinder)
{
	resBinder->AddMockTextureKeys({
		{ "One32", { L"Texture512.png", TextureSlice::One, { { 0, 0, 32, 32 } } } }
		});
}