#include "pch.h"
#include "UIModuleT.h"
#include "Mocks/Stubs/UIComponentStub.h"
#include "Mocks/MockUtils.h"

class MockClickable : public UIComponentStub
{
public:
	MockClickable()
	{
		ON_CALL(*this, OnPress()).WillByDefault(testing::Return(true));
		ON_CALL(*this, OnHold(testing::_)).WillByDefault(testing::Return(true));
		ON_CALL(*this, OnRelease(testing::_)).WillByDefault(testing::Return(true));
	}

	MOCK_METHOD(bool, OnHover, (), (noexcept));
	MOCK_METHOD(bool, OnPress, (), (noexcept));
	MOCK_METHOD(bool, OnHold, (bool inside), (noexcept));
	MOCK_METHOD(bool, OnRelease, (bool inside), (noexcept));
};

namespace UserInterfaceT
{
	TEST_F(UIModuleT, UpdateMouseState_Inside)
	{
		auto comp = CreateOneLevelComponent<MockClickable>(m_main);

		testing::InSequence seq; //호출순서 검증
		EXPECT_CALL(*comp, OnPress()).Times(1);
		EXPECT_CALL(*comp, OnHold(true)).Times(1);
		EXPECT_CALL(*comp, OnRelease(true)).Times(1);

		SimulateClick(comp->GetLeftTop());
	}
}