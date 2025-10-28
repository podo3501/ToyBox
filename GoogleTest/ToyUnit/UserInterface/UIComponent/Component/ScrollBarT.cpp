#include "pch.h"
#include "ScrollBarT.h"
#include "UserInterface/UIComponent/Component/ComponentHelper.h"

namespace UserInterface::UIComponentT::ComponentT
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

		MockTextureRender render;
		EXPECT_CALL(render, Render(testing::_, testing::_, testing::_))
			.WillRepeatedly(RenderLogger(L"ScrollBarT"));

		m_component->UpdatePositionsManually();
		m_component->ProcessRender(&render);

		//dest: (0, 95, 20, 100)
		//source : (0, 45, 20, 50)

		//dest : (0, 46, 20, 50)
		//source : (0, 16, 20, 20)

		/*MockTextureRender render;
		EXPECT_CALL(render, Render(testing::_, testing::_, testing::_))
			.WillRepeatedly([](int index, const RECT& dest, const RECT* source) {
				int a = 1;
				});*/

		//MockTextureRender render;
		//EXPECT_CALL(render, Render(0, dest, ::testing::Pointee(source)))
		//	.Times(1);


	}


		//m_component->ChangeSize({ 20, 200 });

		//testing::MockFunction<void(float)> ScrollChangedCB;
		//m_component->AddScrollChangedCB(ScrollChangedCB.AsStdFunction());

		//uint32_t viewArea = 500;
		//uint32_t contentSize = 2000;
		//m_component->UpdateScrollView(viewArea, contentSize); //?!? Update�̸��� ���ϱ� �򰥸��µ� �ϴ�. �̸� �������
		//m_component->SetPositionRatio(0.5f); // ��ũ�� ��ư�� �߰����� ��ġ��Ŵ
		
	//}

	TEST_F(ScrollBarT, WriteAndRead)
	{
		EXPECT_TRUE(TestWriteAndRead(m_component, GetTempDir() + L"ScrollBarT_WR.json",
			GetResBinder(), GetTextureController()));
	}
}