#pragma once
#include "Mocks/MockRenderer.h"
#include "UserInterface/TextureResourceBinder/MockTextureResourceBinder.h"
#include "Shared/Framework/EnvironmentLocator.h"
#include "Toy/UserInterface/UIComponent/UIType.h"
#include "Toy/UserInterface/UIComponent/Components/PatchTexture/PatchTextureStd/PatchTextureStd1.h"

class PatchTextureStd1T : public ::testing::Test
{
protected:
	virtual void SetUp() override;
	virtual void TearDown() override {};
	ITextureController* GetTextureController() const noexcept { return m_renderer->GetTextureController(); }

private:
	void SetupTestResource();

	unique_ptr<Environment> m_environment;
	unique_ptr<MockRenderer> m_renderer;

protected: //선언순서의 역순으로 소멸되기 때문에 protected를 밑으로 배치함.
	unique_ptr<MockTextureResourceBinder> m_resBinder;
	unique_ptr<PatchTextureStd1> m_component;
};

///////////////////////////////////////////////////////////////

void PatchTextureStd1T::SetUp()
{
	SetupTestResource();

	m_component = CreateComponent<PatchTextureStd1>("One32");
	m_component->BindTextureSourceInfo(m_resBinder.get(), GetTextureController());
}

void PatchTextureStd1T::SetupTestResource()
{
	m_environment = InitializeEnvironment(L"", { 800.f, 600.f });
	m_renderer = make_unique<MockRenderer>();
	m_renderer->RegisterMockTextureInfos({
		{ L"Texture512.png", 0, {512, 512} }
		});

	m_resBinder = make_unique<MockTextureResourceBinder>();
	m_resBinder->RegisterMockTextures({
		{ "One32", { L"Texture512.png", TextureSlice::One, { { 0, 0, 32, 32 } } } },
		{ "One64", { L"Texture512.png", TextureSlice::One, { { 0, 0, 64, 64 } } } },
		});

	m_renderer->LoadTextureBinder(m_resBinder.get());
}
