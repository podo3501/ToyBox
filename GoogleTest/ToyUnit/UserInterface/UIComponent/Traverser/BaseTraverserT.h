#pragma once
#include "../MockComponent.h"
#include "Internal/MockRenderer.h"
#include "Toy/UserInterface/UIComponentLocator.h"

class BaseTraverserT : public testing::Test
{
protected:
	virtual void SetUp() override;
	virtual void TearDown() override {};

	unique_ptr<UIComponentManager> m_componentManager;

private:
};

void BaseTraverserT::SetUp()
{
	MockRenderer renderer;
	m_componentManager = make_unique<UIComponentManager>(&renderer);
	Locator<UIComponentManager>::Provide(m_componentManager.get());
}
