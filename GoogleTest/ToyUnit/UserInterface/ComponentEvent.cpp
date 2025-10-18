#include "pch.h"
#include "Fixture/FixtureSuite.h"
#include "Mocks/MockInputManager.h"
#include "Shared/Utils/StlExt.h"
#include "Helper.h"
#include "Toy/UserInterface/UIModule.h"
#include "Toy/UserInterface/UIComponent/Components/PatchTexture/PatchTextureStd/PatchTextureStd1.h"
#include "Toy/UserInterface/UIComponent/Components/SampleComponent.h"
#include "Toy/UserInterface/UIComponent/Components/TextureSwitcher.h"
#include "Toy/System/EventDispatcher.h"

class MockMouseClicked : public PatchTextureStd1
{
public:
	MockMouseClicked()
	{
		ON_CALL(*this, OnPress()).WillByDefault(testing::Return(true));
		ON_CALL(*this, OnHold(testing::_)).WillByDefault(testing::Return(true));
		ON_CALL(*this, OnRelease(testing::_)).WillByDefault([this](bool inside) {
			if (inside)
				EventDispatcher::Dispatch("region", "MockMouseClicked", UIEvent::Clicked);
			return true; });
	}

	MOCK_METHOD(bool, OnPress, (), (noexcept));
	MOCK_METHOD(bool, OnHold, (bool inside), (noexcept));
	MOCK_METHOD(bool, OnRelease, (bool inside), (noexcept));
};

static MockMouseClicked* CreateMockMouseClicked(UIModule* uiModule, const XMINT2& pos)
{
	auto [comp, compPtr] = GetPtrs(CreateComponent<MockMouseClicked>("BackImage1"));
	UIEx(uiModule->GetMainPanel()).AttachComponent(move(comp), pos);
	uiModule->BindTextureResources();
	compPtr->ChangeOrigin(Origin::LeftTop);
	return compPtr;
}

namespace UserInterface
{
	TEST_F(ComponentEvent, EventDispatch)
	{
		constexpr XMINT2 compPos{ 400, 300 };
		auto compPtr = CreateMockMouseClicked(m_uiModule.get(), compPos);

		testing::MockFunction<void(UIEvent)> mockCallback;
		EXPECT_CALL(mockCallback, Call(UIEvent::Clicked)).Times(1);
		EventDispatcher::Subscribe("region", "MockMouseClicked", mockCallback.AsStdFunction());

		SimulateMouse(compPos, InputState::Pressed);
		SimulateMouse(compPos, InputState::Held);
		SimulateMouse(compPos, InputState::Released);

		//SimulateClick(compPos, compPos);
		EventDispatcher::Clear(); 
		//디스패쳐를 locator로 만들고 ComponentEvent가 eventDispatcher를 가지고 있게
		//어떤 클래스가 locator인지 어떻게 알지? 구분하는 방법을 고민
		//locator를 pch에 넣어야 할지 고민
		//클릭함수 옵셔널로 수정
	}

	TEST_F(ComponentEvent, MouseClickedInside) //안에서 클릭해서 안에서 뗄때.
	{
		constexpr XMINT2 compPos{ 400, 300 };
		auto compPtr = CreateMockMouseClicked(m_uiModule.get(), compPos);

		//호출순서 검증
		testing::InSequence seq;

		EXPECT_CALL(*compPtr, OnPress()).Times(1);
		EXPECT_CALL(*compPtr, OnHold(true)).Times(1);
		EXPECT_CALL(*compPtr, OnRelease(true)).Times(1);

		SimulateMouse(compPos, InputState::Pressed);
		SimulateMouse(compPos, InputState::Held);
		SimulateMouse(compPos, InputState::Released);

		//SimulateClick(compPos, compPos);
	}

	TEST_F(ComponentEvent, MouseClickedOutside) //안에서 클릭해서 밖에서 버튼을 뗄때.
	{
		constexpr XMINT2 compPos{ 400, 300 }, outsidePos{ 0, 0 };
		auto compPtr = CreateMockMouseClicked(m_uiModule.get(), compPos);

		//호출순서 검증
		testing::InSequence seq;

		EXPECT_CALL(*compPtr, OnPress()).Times(1);
		EXPECT_CALL(*compPtr, OnHold(false)).Times(1);
		EXPECT_CALL(*compPtr, OnRelease(false)).Times(1);

		SimulateMouse(compPos, InputState::Pressed);
		SimulateMouse(outsidePos, InputState::Held);
		SimulateMouse(outsidePos, InputState::Released);
	}

	class MockMouseHover : public PatchTextureStd1
	{
	public:
		MOCK_METHOD(bool, OnHover, (), (noexcept));
	};

	TEST_F(ComponentEvent, MouseHover)
	{
		auto [mockComp1, mockComp1Ptr] = GetPtrs(CreateComponent<MockMouseHover>("BackImage1"));
		auto [mockComp2, mockComp2Ptr] = GetPtrs(CreateComponent<MockMouseHover>("BackImage1"));
		UIEx(mockComp1).AttachComponent(move(mockComp2), { 0, 0 });
		UIEx(m_main).AttachComponent(move(mockComp1), { 400, 300 });
		m_uiModule->BindTextureResources();
		mockComp1Ptr->ChangeOrigin(Origin::LeftTop);

		//마우스를 올리고 hover 되는지 확인.
		EXPECT_CALL(*mockComp1Ptr, OnHover()).Times(1);
		EXPECT_CALL(*mockComp2Ptr, OnHover()).Times(1);

		SimulateMouse(401, 301, InputState::Pressed);
	}

	//?!? 이렇게 실제로 TextureSwitcher를 쓰지말고 간단한 컴포넌트를 상속받아서 테스트하면 될것이다.
	TEST_F(ComponentEvent, TextureSwitcher)
	{
		auto [switcher, switcherPtr] = GetPtrs(CreateComponent<TextureSwitcher>(TextureSlice::One,
			GetStateKeyMap("ExitButton1"), BehaviorMode::Normal));
		UIEx(m_main).AttachComponent(move(switcher), { 100, 100 });
		m_uiModule->BindTextureResources();

		switcherPtr->ChangeOrigin(Origin::Center);
		EXPECT_TRUE((switcherPtr->GetSize() == XMUINT2{ 32, 32 }));

		//이벤트를 등록해서 날라오는지 확인한다.
		testing::MockFunction<void(UIEvent)> mockCallback;
		EXPECT_CALL(mockCallback, Call(UIEvent::Clicked)).Times(1);
		EventDispatcher::Subscribe("", "TextureSwitcher", mockCallback.AsStdFunction());

		//눌렀다 뗄때 이벤트가 날라간다.
		m_mockInput->SetMouseState(100, 100, InputState::Pressed);
		m_main->ProcessUpdate(m_timer);
		m_mockInput->SetMouseState(100, 100, InputState::Released);
		m_main->ProcessUpdate(m_timer);
	}
}