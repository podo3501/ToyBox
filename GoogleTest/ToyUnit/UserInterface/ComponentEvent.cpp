#include "pch.h"
#include "Fixture/FixtureSuite.h"
#include "Shared/Utils/StlExt.h"
#include "Helper.h"
#include "Toy/UserInterface/UIModule.h"
#include "Toy/UserInterface/UIComponent/Components/PatchTexture/PatchTextureStd/PatchTextureStd1.h"
#include "Toy/UserInterface/UIComponent/Components/SampleComponent.h"
#include "Toy/UserInterface/UIComponent/Components/TextureSwitcher.h"
#include "Toy/System/EventDispatcher.h"

namespace UserInterface
{
	TEST_F(ComponentEvent, EventDispatch)
	{
		testing::MockFunction<void(UIEvent)> mockCallback;
		//Call�� �Ǿ�� �Ѵ�.
		EXPECT_CALL(mockCallback, Call(UIEvent::Clicked)).Times(1);
		//����Ѵ�.
		EventDispatcher::Subscribe("region", "name", mockCallback.AsStdFunction());
		//�����ϸ�
		EventDispatcher::Dispatch("region", "name", UIEvent::Clicked);
	}

	//?!? �̷��� ������ TextureSwitcher�� �������� ������ ������Ʈ�� ��ӹ޾Ƽ� �׽�Ʈ�ϸ� �ɰ��̴�.
	TEST_F(ComponentEvent, TextureSwitcher)
	{
		auto [switcher, switcherPtr] = GetPtrs(CreateComponent<TextureSwitcher>(TextureSlice::One,
			GetStateKeyMap("ExitButton1"), BehaviorMode::Normal));
		UIEx(m_main).AttachComponent(move(switcher), { 100, 100 });
		m_uiModule->BindTextureResources();

		switcherPtr->ChangeOrigin(Origin::Center);
		EXPECT_TRUE((switcherPtr->GetSize() == XMUINT2{ 32, 32 }));

		//�̺�Ʈ�� ����ؼ� ��������� Ȯ���Ѵ�.
		testing::MockFunction<void(UIEvent)> mockCallback;
		EXPECT_CALL(mockCallback, Call(UIEvent::Clicked)).Times(1);
		EventDispatcher::Subscribe("", "TextureSwitcher", mockCallback.AsStdFunction());

		//������ ���� �̺�Ʈ�� ���󰣴�.
		MockMouseInput(100, 100, true);	//Pressed
		m_main->ProcessUpdate(m_timer);
		MockMouseInput(100, 100, false);	//Released
		m_main->ProcessUpdate(m_timer);
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

		//���콺�� �ø��� hover �Ǵ��� Ȯ��.
		EXPECT_CALL(*mockComp1Ptr, OnHover()).Times(1); 
		EXPECT_CALL(*mockComp2Ptr, OnHover()).Times(1);

		MockMouseInput(401, 301, false); //Hover
		m_uiModule->UpdateMouseState();
	}

	class MockMouseClicked : public PatchTextureStd1
	{
	public:
		MockMouseClicked()
		{
			ON_CALL(*this, OnPress()).WillByDefault(testing::Return(true));
			ON_CALL(*this, OnHold()).WillByDefault(testing::Return(true));
			ON_CALL(*this, OnRelease()).WillByDefault(testing::Return(true));
		}

		MOCK_METHOD(bool, OnPress, (), (noexcept));
		MOCK_METHOD(bool, OnHold, (), (noexcept));
		MOCK_METHOD(bool, OnRelease, (), (noexcept));
	};

	TEST_F(ComponentEvent, MouseClickedInside)
	{
		auto [comp, compPtr] = GetPtrs(CreateComponent<MockMouseClicked>("BackImage1"));
		UIEx(m_main).AttachComponent(move(comp), { 400, 300 });
		m_uiModule->BindTextureResources();
		compPtr->ChangeOrigin(Origin::LeftTop);

		EXPECT_CALL(*compPtr, OnPress()).Times(1);
		EXPECT_CALL(*compPtr, OnHold()).Times(1);
		//EXPECT_CALL(*compPtr, OnRelease()).Times(1);

		MockMouseInput(401, 301, true); //Pressed
		m_uiModule->UpdateMouseState();
		MockMouseInput(401, 301, true); //Held
		m_uiModule->UpdateMouseState();
		MockMouseInput(401, 301, false); //Released
		m_uiModule->UpdateMouseState();
	}

}