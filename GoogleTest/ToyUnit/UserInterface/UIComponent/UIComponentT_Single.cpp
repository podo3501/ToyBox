#include "pch.h"
#include "UIComponentT_Single.h"

namespace UserInterface::UIComponentT
{
	//Attach 테스트할 예정
	TEST_F(UIComponentT_Single, Attach)
	{
		unique_ptr<UIComponent> result = UIEx(m_main).AttachComponent(move(m_mock));

		EXPECT_EQ(result, nullptr);
	}

	TEST_F(UIComponentT_Single, Detach)
	{
		UIEx(m_main).AttachComponent(move(m_mock));
		auto [detach, detachPtr] = UIEx(m_mockPtr).DetachComponent();
		
		EXPECT_NE(detach, nullptr); //detach가 잘 되면 detach된 component가 반환된다.
		EXPECT_NE(detachPtr, nullptr);
	}

	TEST_F(UIComponentT_Single, StateFlag_Attach)
	{
		m_main->SetStateFlag(StateFlag::Attach, false);	//attach 불가라면
		unique_ptr<UIComponent> result = UIEx(m_main).AttachComponent(move(m_mock));

		EXPECT_NE(result, nullptr);
	}

	TEST_F(UIComponentT_Single, StateFlag_Detach)
	{
		UIEx(m_main).AttachComponent(move(m_mock));
		m_main->SetStateFlag(StateFlag::Detach, false);
		auto [detach, detachPtr] = UIEx(m_mockPtr).DetachComponent();

		EXPECT_EQ(detach, nullptr); //detach가 안되면 nullptr이 반환된다.
		EXPECT_EQ(detachPtr, nullptr);
	}

	TEST_F(UIComponentT_Single, StateFlag_SizeLocked)
	{
		XMUINT2 curSize{ m_mock->GetSize() }, newSize{ 128, 128 };
		m_main->SetStateFlag(StateFlag::X_SizeLocked, true);
		m_main->SetStateFlag(StateFlag::Y_SizeLocked, true);
		m_main->ChangeSize(newSize);

		EXPECT_EQ(m_mock->GetSize(), curSize);
	}
}