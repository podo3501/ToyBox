#include "pch.h"
#include "DerivedTraverserT.h"
#include "../MockComponent.h"
#include "Toy/UserInterface/UIComponent/Traverser/UITraverser.h"

using namespace UITraverser;

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

namespace UserInterfaceT::UIComponentT::TraverserT
{
	TEST_F(DerivedTraverserT, EnableToolMode_false)
	{
		auto [owner, parent] = CreateMockComponent<MockToolMode>();
		auto child = AttachMockComponentDirect<MockToolMode>(parent);

		testing::InSequence seq;
		EXPECT_CALL(*child, ExitToolMode()).Times(1);
		EXPECT_CALL(*parent, ExitToolMode()).Times(1);
		
		m_traverser.EnableToolMode(parent, true); //초기값이 false 이기 때문에 먼저 true 해주고 false를 해야 한다.
		m_traverser.EnableToolMode(parent, false);
	}

	TEST_F(DerivedTraverserT, EnableToolMode_true)
	{
		auto [owner, parent] = CreateMockComponent<MockToolMode>();
		auto child = AttachMockComponentDirect<MockToolMode>(parent);

		//툴 모드를 호출하면 모든 컴포넌트에 ToolMode 함수가 호출
		testing::InSequence seq;
		EXPECT_CALL(*child, EnterToolMode()).Times(1);
		EXPECT_CALL(*parent, EnterToolMode()).Times(1);

		m_traverser.EnableToolMode(parent, true);
	}
}