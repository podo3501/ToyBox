#pragma once
#include "Internal/MockRenderer.h"
#include "Internal/MockInputManager.h"
#include "Shared/Framework/EnvironmentLocator.h"
#include "Toy/UserInterface/UIComponentLocator.h"
#include "Toy/Locator/InputLocator.h"

class UIComponentManagerT : public ::testing::Test
{
protected:
	virtual void SetUp() override;
	virtual void TearDown() override {};
	virtual void RegisterRenderTextures(MockRenderer* renderer);
	inline wstring GetTempDir() const noexcept { return L"../Resources/Test/Temp/"; }

	unique_ptr<Environment> m_environment;
	unique_ptr<MockInputManager> m_input;
	unique_ptr<MockRenderer> m_renderer;
	unique_ptr<UIComponentManager> m_componentManager;
};