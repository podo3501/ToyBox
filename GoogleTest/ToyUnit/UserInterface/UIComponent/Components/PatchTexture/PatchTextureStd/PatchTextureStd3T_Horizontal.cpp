#include "pch.h"
#include "PatchTextureStd3T_Horizontal.h"
#include "UserInterface/UIComponent/Components/ComponentHelper.h"
#include "Shared/Utils/GeometryExt.h"

namespace UserInterfaceT::UIComponentT::ComponentT::PatchTextureT
{
	TEST_F(PatchTextureStd3T_Horizontal, ChangeBindKey)
	{
		m_component->ChangeBindKey(GetResBinder(), "ThreeH60");
		EXPECT_EQ(m_component->GetArea(), Rectangle(0, 0, 60, 60));
	}

	TEST_F(PatchTextureStd3T_Horizontal, ChangeSize_Bigger)
	{
		m_component->ChangeSize({ 80, 80 });
		auto centerComponent = m_component->GetCenterComponent();
		EXPECT_EQ(centerComponent->GetSize(), XMUINT2(36, 80)); // 80 - (22 * 2) = 36, ���� ������ �簢�� x���̴� ������ �ʴ´�.
	}

	//?!? �۰� ����� �ϸ� ���װ� ����
	//TEST_F(PatchTextureStd3T, ChangeSize_Smaller)
	//{
		//m_component->ChangeSize({ 30, 30 });
		//auto preSize = m_component->GetSize();
	//}

	TEST_F(PatchTextureStd3T_Horizontal, Clone)
	{
		EXPECT_TRUE(TestClone(m_component));
	}

	TEST_F(PatchTextureStd3T_Horizontal, FitToTextureSource)
	{
		auto preSize = m_component->GetSize();
		m_component->ChangeSize({ 60, 60 });

		m_component->FitToTextureSource();
		EXPECT_EQ(m_component->GetSize(), preSize);
	}

	TEST_F(PatchTextureStd3T_Horizontal, GetCenterComponent)
	{
		auto centerComponent = m_component->GetCenterComponent();
		EXPECT_EQ(centerComponent->GetArea(), Rectangle(22, 0, 4, 48));
	}

	TEST_F(PatchTextureStd3T_Horizontal, GetTextureSlice)
	{
		EXPECT_EQ(m_component->GetTextureSlice(), TextureSlice::ThreeH);
	}

	TEST_F(PatchTextureStd3T_Horizontal, ProcessRender)
	{
		m_component->ChangeOrigin(Origin::Center);
		m_component->ChangeSize({ 80, 80 }); //source ��ǥ�� �����ؼ� ����� �����ߴ�.

		//ȭ�鿡 �׷����� �κ�
		RECT destLeft{ -40, -40, -18, 40 };
		RECT destCenter{ -18, -40, 18, 40 };
		RECT destRight{ 18, -40, 40, 40 };

		//�ؽ��Ŀ��� �������� ����
		RECT srcLeft{ 0, 0, 22, 48 };
		RECT srcCenter{ 22, 0, 26, 48 };
		RECT srcRight{ 26, 0, 48, 48 };

		MockTextureRender render;
		::testing::InSequence seq;
		EXPECT_CALL(render, Render(0, destLeft, ::testing::Pointee(srcLeft))).Times(1);
		EXPECT_CALL(render, Render(0, destCenter, ::testing::Pointee(srcCenter))).Times(1);
		EXPECT_CALL(render, Render(0, destRight, ::testing::Pointee(srcRight))).Times(1);

		m_component->UpdatePositionsManually();
		m_component->ProcessRender(&render);
	}

	TEST_F(PatchTextureStd3T_Horizontal, WriteAndRead)
	{
		EXPECT_TRUE(TestWriteAndRead(m_component, GetTempDir() + L"PatchTextureStd3T_Horizontal_WR.json"));
	}
}