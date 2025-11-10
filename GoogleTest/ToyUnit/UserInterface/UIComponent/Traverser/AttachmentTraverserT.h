#pragma once
#include "Toy/UserInterface/UIComponentLocator.h"

class AttachmentTraverserT : public testing::Test
{
protected:
	virtual void SetUp() override;
	virtual void TearDown() override {};

	unique_ptr<UIComponentManager> m_componentManager;
};

void AttachmentTraverserT::SetUp()
{
	m_componentManager = make_unique<UIComponentManager>();
	Locator<UIComponentManager>::Provide(m_componentManager.get());
}
