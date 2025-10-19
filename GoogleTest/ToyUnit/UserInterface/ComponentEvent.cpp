#include "pch.h"
#include "Fixture/FixtureSuite.h"
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
		auto eventDispatcher = EventDispatcherLocator::GetService();

		constexpr XMINT2 compPos{ 400, 300 };
		auto [comp, compPtr] = CreateMockComponent<MockMouseClicked>();
		UIEx(m_main).AttachComponent(move(comp), compPos);

		testing::MockFunction<void(UIEvent)> mockCallback;
		EXPECT_CALL(mockCallback, Call(UIEvent::Clicked)).Times(1);
		eventDispatcher->Subscribe("region", "MockMouseClicked", mockCallback.AsStdFunction());

		SimulateClick(compPos);
	}

	TEST_F(ComponentEvent, MouseClickedInside) //�ȿ��� Ŭ���ؼ� �ȿ��� ����.
	{
		constexpr XMINT2 compPos{ 400, 300 };
		auto [comp, compPtr] = CreateMockComponent<MockMouseClicked>();
		UIEx(m_main).AttachComponent(move(comp), compPos);

		testing::InSequence seq; //ȣ����� ����
		EXPECT_CALL(*compPtr, OnPress()).Times(1);
		EXPECT_CALL(*compPtr, OnHold(true)).Times(1);
		EXPECT_CALL(*compPtr, OnRelease(true)).Times(1);

		SimulateClick(compPos);
	}

	TEST_F(ComponentEvent, MouseClickedOutside) //�ȿ��� Ŭ���ؼ� �ۿ��� ��ư�� ����.
	{
		constexpr XMINT2 compPos{ 400, 300 }, outsidePos{ 0, 0 };
		auto [comp, compPtr] = CreateMockComponent<MockMouseClicked>();
		UIEx(m_main).AttachComponent(move(comp), compPos);
		
		testing::InSequence seq; //ȣ����� ����
		EXPECT_CALL(*compPtr, OnPress()).Times(1);
		EXPECT_CALL(*compPtr, OnHold(false)).Times(1);
		EXPECT_CALL(*compPtr, OnRelease(false)).Times(1);

		SimulateClick(compPos, outsidePos);
	}

	TEST_F(ComponentEvent, MouseHover)
	{
		constexpr XMINT2 compPos{ 400, 300 };
		auto [comp1, comp1Ptr] = CreateMockComponent<MockMouseClicked>();
		auto [comp2, comp2Ptr] = CreateMockComponent<MockMouseClicked>();
		UIEx(comp1).AttachComponent(move(comp2), { 0, 0 });
		UIEx(m_main).AttachComponent(move(comp1), { 400, 300 });

		//���콺�� �ø��� hover �Ǵ��� Ȯ��.
		EXPECT_CALL(*comp1Ptr, OnHover()).Times(1);
		EXPECT_CALL(*comp2Ptr, OnHover()).Times(1);

		SimulateMouse(compPos, InputState::Pressed);
	}

	//?!? �̰� ���߿� TextureSwitcher ������Ʈ �׽�Ʈ�� �־�� �Ѵ�.
	TEST_F(ComponentEvent, TextureSwitcher)
	{
		auto eventDispatcher = EventDispatcherLocator::GetService();

		auto [switcher, switcherPtr] = GetPtrs(CreateComponent<TextureSwitcher>(TextureSlice::One,
			GetStateKeyMap("ExitButton1"), BehaviorMode::Normal));
		UIEx(m_main).AttachComponent(move(switcher), { 100, 100 });
		m_uiModule->BindTextureResources();

		switcherPtr->ChangeOrigin(Origin::Center);
		EXPECT_TRUE((switcherPtr->GetSize() == XMUINT2{ 32, 32 }));

		//�̺�Ʈ�� ����ؼ� ��������� Ȯ���Ѵ�.
		testing::MockFunction<void(UIEvent)> mockCallback;
		EXPECT_CALL(mockCallback, Call(UIEvent::Clicked)).Times(1);
		eventDispatcher->Subscribe("", "TextureSwitcher", mockCallback.AsStdFunction());

		//������ ���� �̺�Ʈ�� ���󰣴�.
		m_mockInput->SetMouseState(100, 100, InputState::Pressed);
		m_main->ProcessUpdate(m_timer);
		m_mockInput->SetMouseState(100, 100, InputState::Released);
		m_main->ProcessUpdate(m_timer);
	}
}