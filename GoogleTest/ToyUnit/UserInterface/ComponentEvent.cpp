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
		//Call이 되어야 한다.
		EXPECT_CALL(mockCallback, Call(UIEvent::Clicked)).Times(1);
		//등록한다.
		EventDispatcher::Subscribe("region", "name", mockCallback.AsStdFunction());
		//실행하면
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

		//이벤트를 등록해서 날라오는지 확인한다.
		testing::MockFunction<void(UIEvent)> mockCallback;
		EXPECT_CALL(mockCallback, Call(UIEvent::Clicked)).Times(1);
		EventDispatcher::Subscribe("", "TextureSwitcher", mockCallback.AsStdFunction());

		//눌렀다 뗄때 이벤트가 날라간다.
		MockMouseInput(100, 100, true);	//Pressed
		m_main->ProcessUpdate(m_timer);
		MockMouseInput(100, 100, false);	//Released
		m_main->ProcessUpdate(m_timer);
	}

	class MockPatchTextureStd1 : public PatchTextureStd1 
	{
	public:
		MOCK_METHOD(bool, OnHover, (), (noexcept));
	};
	TEST_F(ComponentEvent, MouseEvent)
	{
		auto [mockComp, mockCompPtr] = GetPtrs(CreateComponent<MockPatchTextureStd1>("BackImage1"));
		UIEx(m_main).AttachComponent(move(mockComp), { 400, 300 });
		m_uiModule->BindTextureResources();

		//마우스를 올리고 클릭할때 함수가 호출되는지 확인한다.
		//EXPECT_CALL(*mockCompPtr, OnHover()).Times(1);
	}
}