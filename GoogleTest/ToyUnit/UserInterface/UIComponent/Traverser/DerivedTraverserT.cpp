#include "pch.h"
#include "DerivedTraverserT.h"
#include "../MockComponent.h"

class MockToolMode : public UIComponentStub
{
public:
	explicit MockToolMode()
	{
		ON_CALL(*this, EnterToolMode()).WillByDefault(testing::Return(true));
		ON_CALL(*this, ExitToolMode()).WillByDefault(testing::Return(true));
	}

	MOCK_METHOD(bool, EnterToolMode, (), (noexcept));
	MOCK_METHOD(bool, ExitToolMode, (), (noexcept));
};

namespace D::UserInterface::UIComponent::Traverser
{
	TEST_F(DerivedTraverserT, EnableToolMode_false)
	{
		auto [owner, component] = CreateMockComponent<testing::NiceMock<MockToolMode>>();
		m_traverser.EnableToolMode(component, true); //초기값이 false 이기 때문에 먼저 true 해주고 false를 해야 한다.
		EXPECT_CALL(*component, ExitToolMode()).Times(1);
		
		m_traverser.EnableToolMode(component, false);
	}

	TEST_F(DerivedTraverserT, EnableToolMode_true)
	{
		auto [owner, component] = CreateMockComponent<MockToolMode>();
		EXPECT_CALL(*component, EnterToolMode()).Times(1);

		m_traverser.EnableToolMode(component, true);
	}
}