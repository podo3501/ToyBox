#pragma once
#include "Toy/Locator/EventDispatcherLocator.h"

class EventDispatcherManagerT : public ::testing::Test
{
protected:
	virtual void SetUp() override;
	virtual void TearDown() override {};

	unique_ptr<IEventDispatcherManager> m_eventDispatcher;
	testing::MockFunction<void(UIEvent)> m_mockCallback;
};

void EventDispatcherManagerT::SetUp()
{
	m_eventDispatcher = CreateEventDispatcherManager();
	EventDispatcherLocator::Provide(m_eventDispatcher.get());

	m_eventDispatcher->Subscribe("region", "component", m_mockCallback.AsStdFunction());
}
