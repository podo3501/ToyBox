#include "pch.h"
#include "EventDispatcherManagerT.h"

namespace SystemT
{
	TEST_F(EventDispatcherManagerT, Dispatch)
	{
		auto eventDispatcher = EventDispatcherLocator::GetService(); //m_eventDispatcher를 쓸수도 있지만, 일반적으로 이렇게 사용한다.
		EXPECT_CALL(m_mockCallback, Call(UIEvent::Clicked)).Times(1);

		//Subscribe해서 등록하면 Dispatch로 메세지를 날릴 수 있다.
		eventDispatcher->Dispatch("region", "component", UIEvent::Clicked);
	}

	TEST_F(EventDispatcherManagerT, Clear)
	{
		auto eventDispatcher = EventDispatcherLocator::GetService();
		EXPECT_CALL(m_mockCallback, Call(UIEvent::Clicked)).Times(0);

		//Clear하면 날려도 받지 못한다.
		eventDispatcher->Clear();
		eventDispatcher->Dispatch("region", "component", UIEvent::Clicked);
	}
}