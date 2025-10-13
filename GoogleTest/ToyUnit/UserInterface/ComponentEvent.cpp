#include "pch.h"
#include "Fixture/FixtureSuite.h"
#include "Shared/Utils/StlExt.h"
#include "Helper.h"
#include "Toy/UserInterface/UIModule.h"
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
}