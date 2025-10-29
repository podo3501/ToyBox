#include "pch.h"
#include "ScrollBarT.h"
#include "UserInterface/UIComponent/Component/ComponentHelper.h"
#include "Shared/Utils/GeometryExt.h"

namespace UserInterfaceT::UIComponentT::ComponentT
{
	TEST_F(ScrollBarT, Clone)
	{
		EXPECT_TRUE(TestClone(m_component));
	}

	TEST_F(ScrollBarT, GetSize)
	{
		EXPECT_TRUE((m_component->GetSize() == XMUINT2{ 20, 30 })); //�ڽĵ��� ũ�⿡�� �ʱⰪ�� �����´�.
	}

	TEST_F(ScrollBarT, ProcessRender)
	{
		m_component->ChangeSize({ 20, 100 });

		//ȭ�鿡 �׷����� �κ�
		RECT destTrackBottom{ 0, 95, 20, 100 };
		RECT destButtonBottom{ 0, 46, 20, 50 };

		//�ؽ��Ŀ��� �������� ����
		RECT srcTrackBottom{ 0, 45, 20, 50 };
		RECT srcButtonBottom{ 0, 16, 20, 20 };

		//�� 6���� Render ȣ�������� ���߿� ������ Bottom���� �˻��Ѵ�.
		MockTextureRender render;
		EXPECT_CALL(render, Render).Times(::testing::AnyNumber());
		::testing::InSequence seq;
		EXPECT_CALL(render, Render(0, destTrackBottom, ::testing::Pointee(srcTrackBottom))).Times(1);
		EXPECT_CALL(render, Render(0, destButtonBottom, ::testing::Pointee(srcButtonBottom))).Times(1);

		m_component->UpdatePositionsManually();
		m_component->ProcessRender(&render);
	}

	TEST_F(ScrollBarT, WriteAndRead)
	{
		EXPECT_TRUE(TestWriteAndRead(m_component, GetTempDir() + L"ScrollBarT_WR.json",
			GetResBinder(), GetTextureController()));
	}
}