#include "pch.h"
#include "Fixture/FixtureSuite.h"
#include "Shared/Utils/StlExt.h"
#include "Toy/UserInterface/UIModule.h"
#include "Toy/UserInterface/UIComponent/Components/SampleComponent.h"
#include "Toy/UserInterface/UIComponent/Components/TextureSwitcher.h"

namespace UserInterface
{
	TEST_F(ComponentEvent, EventDispatch)
	{
		auto [switcher, switcherPtr] = GetPtrs(CreateComponent<TextureSwitcher>(TextureSlice::One,
			GetStateKeyMap("ExitButton1"), BehaviorMode::Normal));
		UIEx(m_main).AttachComponent(move(switcher), { 100, 100 });
		m_uiModule->BindTextureResources();

		switcherPtr->ChangeOrigin(Origin::Center);
		EXPECT_TRUE((switcherPtr->GetSize() == XMUINT2{ 32, 32 }));

		MockMouseInput(100, 100, true);	//Pressed
		
		//눌렀다 뗄때 이벤트가 날라간다.
		//이벤트를 등록해서 날라오는지 확인한다.
	}
}