#include "pch.h"
#include "Fixtures/FixtureSuite.h"
#include "Mocks/MockInputManager.h"
#include "Mocks/Stubs/UIComponentStub.h"
#include "Mocks/MockUtils.h"
#include "Shared/Utils/StlExt.h"
#include "Toy/Locator/EventDispatcherLocator.h"
#include "Toy/UserInterface/UIModule.h"
#include "Toy/UserInterface/UIComponent/Components/SampleComponent.h"
#include "Toy/UserInterface/UIComponent/Components/TextureSwitcher.h"

class MockMouseClicked : public UIComponentStub
{
public:
	MockMouseClicked()
	{
		ON_CALL(*this, OnPress()).WillByDefault(testing::Return(true));
		ON_CALL(*this, OnHold(testing::_)).WillByDefault(testing::Return(true));
		ON_CALL(*this, OnRelease(testing::_)).WillByDefault([this](bool inside) {
			if (inside)
			{
				auto eventDispatcher = EventDispatcherLocator::GetService();
				eventDispatcher->Dispatch("region", "MockMouseClicked", UIEvent::Clicked);
			}
			return true; });
	}

	MOCK_METHOD(bool, OnHover, (), (noexcept));
	MOCK_METHOD(bool, OnPress, (), (noexcept));
	MOCK_METHOD(bool, OnHold, (bool inside), (noexcept));
	MOCK_METHOD(bool, OnRelease, (bool inside), (noexcept));
};

namespace UserInterface
{
	TEST_F(ComponentEvent, EventDispatch)
	{
		auto comp = CreateOneLevelComponent<MockMouseClicked>(m_main);

		testing::MockFunction<void(UIEvent)> mockCallback;
		EXPECT_CALL(mockCallback, Call(UIEvent::Clicked)).Times(1);

		auto eventDispatcher = EventDispatcherLocator::GetService();
		eventDispatcher->Subscribe("region", "MockMouseClicked", mockCallback.AsStdFunction());

		SimulateClick(comp->GetLeftTop());
	}

	TEST_F(ComponentEvent, MouseClickedInside) //안에서 클릭해서 안에서 뗄때.
	{
		auto comp = CreateOneLevelComponent<MockMouseClicked>(m_main);

		testing::InSequence seq; //호출순서 검증
		EXPECT_CALL(*comp, OnPress()).Times(1);
		EXPECT_CALL(*comp, OnHold(true)).Times(1);
		EXPECT_CALL(*comp, OnRelease(true)).Times(1);

		SimulateClick(comp->GetLeftTop());
	}

	TEST_F(ComponentEvent, MouseClickedOutside) //안에서 클릭해서 밖에서 버튼을 뗄때.
	{
		XMINT2 outsidePos{ 0, 0 };
		auto comp = CreateOneLevelComponent<MockMouseClicked>(m_main);
		
		testing::InSequence seq; //호출순서 검증
		EXPECT_CALL(*comp, OnPress()).Times(1);
		EXPECT_CALL(*comp, OnHold(false)).Times(1);
		EXPECT_CALL(*comp, OnRelease(false)).Times(1);

		SimulateClick(comp->GetLeftTop(), outsidePos);
	}

	TEST_F(ComponentEvent, MouseHover)
	{
		auto [parent, child] = CreateTwoLevelComponents<MockMouseClicked>(m_main);

		//마우스를 올리고 hover 되는지 확인.
		EXPECT_CALL(*parent, OnHover()).Times(1);
		EXPECT_CALL(*child, OnHover()).Times(1);

		SimulateMouse(child->GetLeftTop(), InputState::Pressed);
	}

	//?!? 이건 나중에 TextureSwitcher 컴포넌트 테스트에 넣어야 한다.
	TEST_F(ComponentEvent, TextureSwitcher)
	{
		auto eventDispatcher = EventDispatcherLocator::GetService();

		auto [switcher, switcherPtr] = GetPtrs(CreateComponent<TextureSwitcher>(TextureSlice::One,
			GetStateKeyMap("ExitButton1"), BehaviorMode::Normal));
		UIEx(m_main).AttachComponent(move(switcher), { 100, 100 });
		m_uiModule->BindTextureResources();

		switcherPtr->ChangeOrigin(Origin::Center);
		EXPECT_TRUE((switcherPtr->GetSize() == XMUINT2{ 32, 32 }));

		//이벤트를 등록해서 날라오는지 확인한다.
		testing::MockFunction<void(UIEvent)> mockCallback;
		EXPECT_CALL(mockCallback, Call(UIEvent::Clicked)).Times(1);
		eventDispatcher->Subscribe("", "TextureSwitcher", mockCallback.AsStdFunction());

		//눌렀다 뗄때 이벤트가 날라간다.
		m_mockInput->SetMouseState(100, 100, InputState::Pressed);
		m_main->ProcessUpdate(m_timer);
		m_mockInput->SetMouseState(100, 100, InputState::Released);
		m_main->ProcessUpdate(m_timer);
	}
}