#include "pch.h"
#include "RenderTextureT.h"
#include "UserInterface/UIComponent/Components/ComponentHelper.h"
#include "Shared/Utils/GeometryExt.h"

namespace UserInterfaceT::UIComponentT::ComponentT
{
	TEST_F(RenderTextureT, GetSize)
	{
		EXPECT_TRUE((m_component->GetSize() == XMUINT2{ 32, 32 })); //�ʱ������ target component �� �⺻ ������.
	}

	TEST_F(RenderTextureT, Clone)
	{
		EXPECT_TRUE(TestClone(m_component));
	}

	TEST_F(RenderTextureT, ProcessRender)
	{
		m_component->ChangeSize({ 100, 100 });

		size_t texIndex = 1; //���� ������ �ؽ��Ķ� �ε����� ������.
		RECT dest{ 0, 0, 100, 100 }; //������ ũ�⸸ŭ.
		RECT source{ 0, 0, 100, 100 }; //�ؽ��ĸ� ������ ũ�⸸ŭ ������� ������ ���� ����
		
		MockTextureRender render;
		EXPECT_CALL(render, Render(texIndex, dest, ::testing::Pointee(source))).Times(1);

		m_component->ProcessRender(&render);
	}

	TEST_F(RenderTextureT, WriteAndRead)
	{
		EXPECT_TRUE(TestWriteAndRead(m_component, GetTempDir() + L"RenderTextureT_WR.json"));
	}
}