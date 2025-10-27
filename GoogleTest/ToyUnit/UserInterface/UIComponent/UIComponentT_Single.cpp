#include "pch.h"
#include "UIComponentT_Single.h"

namespace UserInterface::UIComponentT
{
	//Attach �׽�Ʈ�� ����
	TEST_F(UIComponentT_Single, Attach)
	{
		unique_ptr<UIComponent> result = UIEx(m_main).AttachComponent(move(m_mock));

		EXPECT_EQ(result, nullptr);
	}

	TEST_F(UIComponentT_Single, Detach)
	{
		UIEx(m_main).AttachComponent(move(m_mock));
		auto [detach, detachPtr] = UIEx(m_mockPtr).DetachComponent();
		
		EXPECT_NE(detach, nullptr); //detach�� �� �Ǹ� detach�� component�� ��ȯ�ȴ�.
		EXPECT_NE(detachPtr, nullptr);
	}

	TEST_F(UIComponentT_Single, StateFlag_Attach)
	{
		m_main->SetStateFlag(StateFlag::Attach, false);	//attach �Ұ����
		unique_ptr<UIComponent> result = UIEx(m_main).AttachComponent(move(m_mock));

		EXPECT_NE(result, nullptr);
	}

	TEST_F(UIComponentT_Single, StateFlag_Detach)
	{
		UIEx(m_main).AttachComponent(move(m_mock));
		m_main->SetStateFlag(StateFlag::Detach, false);
		auto [detach, detachPtr] = UIEx(m_mockPtr).DetachComponent();

		EXPECT_EQ(detach, nullptr); //detach�� �ȵǸ� nullptr�� ��ȯ�ȴ�.
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