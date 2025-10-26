#include "pch.h"
#include "PatchTextureStd3T_Vertical.h"
#include "Shared/SerializerIO/SerializerIO.h"
#include "Shared/Utils/GeometryExt.h"

namespace UserInterface::UIComponentT::ComponentT
{
	TEST_F(PatchTextureStd3T_Vertical, ChangeSize_Bigger)
	{
		m_component->ChangeSize({ 80, 80 });
		auto centerComponent = m_component->GetCenterComponent();
		EXPECT_EQ(centerComponent->GetSize(), XMUINT2(80, 36)); // 80 - (22 * 2) = 36, ���� �Ʒ��� �簢�� y���̴� ������ �ʴ´�.
	}

	//?!? �۰� ����� �ϸ� ���װ� ����
	//TEST_F(PatchTextureStd3T_Vertical, ChangeSize_Smaller)
	//{
		//m_component->ChangeSize({ 30, 30 });
		//auto preSize = m_component->GetSize();
	//}

	TEST_F(PatchTextureStd3T_Vertical, Clone)
	{
		auto clone = m_component->Clone();
		EXPECT_TRUE(CompareDerived(m_component, clone));
	}

	TEST_F(PatchTextureStd3T_Vertical, FitToTextureSource)
	{
		auto preSize = m_component->GetSize();
		m_component->ChangeSize({ 60, 60 });

		m_component->FitToTextureSource();
		EXPECT_EQ(m_component->GetSize(), preSize);
	}

	TEST_F(PatchTextureStd3T_Vertical, GetCenterComponent)
	{
		auto centerComponent = m_component->GetCenterComponent();
		EXPECT_EQ(centerComponent->GetArea(), Rectangle(0, 22, 48, 4));
	}

	TEST_F(PatchTextureStd3T_Vertical, GetTextureSlice)
	{
		EXPECT_EQ(m_component->GetTextureSlice(), TextureSlice::ThreeV);
	}

	TEST_F(PatchTextureStd3T_Vertical, ProcessRender)
	{
		m_component->ChangeOrigin(Origin::Center);
		m_component->ChangeSize({ 80, 80 }); //source ��ǥ�� �����ؼ� ����� �����ߴ�.

		//ȭ�鿡 �׷����� �κ�
		RECT destTop{ -40, -40,  40, -18 };
		RECT destMiddle{ -40, -18,  40,  18 };
		RECT destBottom{ -40,  18,  40,  40 };

		//�ؽ��Ŀ��� �������� ����
		RECT srcTop{ 0,  0, 48, 22 };
		RECT srcMiddle{ 0, 22, 48, 26 };
		RECT srcBottom{ 0, 26, 48, 48 };

		MockTextureRender render;
		::testing::InSequence seq;
		EXPECT_CALL(render, Render(0, destTop, ::testing::Pointee(srcTop)));
		EXPECT_CALL(render, Render(0, destMiddle, ::testing::Pointee(srcMiddle)));
		EXPECT_CALL(render, Render(0, destBottom, ::testing::Pointee(srcBottom)));

		m_component->UpdatePositionsManually();
		m_component->ProcessRender(&render);
	}

	TEST_F(PatchTextureStd3T_Vertical, WriteAndRead)
	{
		wstring filename = L"../Resources/Test/Temp/PatchtextureStd3T_Vertical_WriteAndRead.json";
		EXPECT_TRUE(SerializerIO::WriteJsonToFile(m_component, filename));

		unique_ptr<PatchTextureStd3> read;
		EXPECT_TRUE(SerializerIO::ReadJsonFromFile(filename, read));

		EXPECT_TRUE(CompareDerived(m_component, read));
	}
}