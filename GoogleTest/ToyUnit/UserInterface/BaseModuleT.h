#pragma once
#include "Toy/UserInterface/TextureResourceBinder/TextureResourceBinder.h"
#include "Toy/UserInterface/UIModule.h"
#include "Internal/MockRenderer.h"
#include "Shared/Framework/EnvironmentLocator.h"

class BaseModuleT : public ::testing::Test
{
protected:
	virtual void SetUp() override;
	virtual void TearDown() override {};
	virtual void RegisterRenderTextures(MockRenderer* renderer);
	virtual void RegisterBinderTextures(TextureResourceBinder* resBinder) {};
	inline wstring GetTempDir() const noexcept { return L"../Resources/Test/Temp/"; }

	unique_ptr<Environment> m_environment;
	unique_ptr<MockRenderer> m_renderer;
	unique_ptr<UIModule> m_uiModule;
	UIComponent* m_main{ nullptr };
};
