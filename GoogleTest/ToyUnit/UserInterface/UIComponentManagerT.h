#pragma once
#include "Internal/MockInputManager.h"
#include "Internal/MockRenderer.h"
#include "Toy/UserInterface/UIComponentLocator.h"
#include "Toy/Locator/InputLocator.h"

class UIComponentManagerT : public ::testing::Test
{
protected:
	virtual void SetUp() override;
	virtual void TearDown() override {};
	virtual void RegisterRenderTextures(MockRenderer* renderer);

	unique_ptr<MockInputManager> m_input;
	unique_ptr<MockRenderer> m_renderer;
	unique_ptr<UIComponentManager> m_componentManager;
};