#include "pch.h"
#include "PatchTextureStd9T.h"
#include "UserInterface/UIComponent/Component/ComponentHelper.h"
#include "Shared/Utils/GeometryExt.h"

namespace UserInterface::UIComponentT::ComponentT::PatchTextureT
{
	TEST_F(PatchTextureStd9T, ChangeBindKey)
	{
		m_component->ChangeBindKey(GetResBinder(), "Nine60");
		EXPECT_EQ(m_component->GetArea(), Rectangle(0, 0, 60, 60));
	}

	TEST_F(PatchTextureStd9T, ChangeSize_Bigger)
	{
		m_component->ChangeSize({ 80, 80 });
		auto centerComponent = m_component->GetCenterComponent();
		EXPECT_EQ(centerComponent->GetSize(), XMUINT2(36, 36)); // 80 - (22 * 2) = 36, ���� ������ �� �Ʒ� �簢�� x�Ǵ� y����(22)�� ������ �ʴ´�.
	}

	//?!? �۰� ����� �ϸ� ���װ� ����
	//TEST_F(PatchTextureStd9T, ChangeSize_Smaller)
	//{
		//m_component->ChangeSize({ 30, 30 });
		//auto preSize = m_component->GetSize();
	//}

	TEST_F(PatchTextureStd9T, Clone)
	{
		auto clone = m_component->Clone();
		EXPECT_TRUE(CompareDerived(m_component, clone));
	}

	TEST_F(PatchTextureStd9T, FitToTextureSource)
	{
		auto preSize = m_component->GetSize();
		m_component->ChangeSize({ 60, 60 });

		m_component->FitToTextureSource();
		EXPECT_EQ(m_component->GetSize(), preSize);
	}

	TEST_F(PatchTextureStd9T, GetCenterComponent)
	{
		auto centerComponent = m_component->GetCenterComponent();
		EXPECT_EQ(centerComponent->GetArea(), Rectangle(22, 22, 4, 4));
	}

	TEST_F(PatchTextureStd9T, GetTextureSlice)
	{
		EXPECT_EQ(m_component->GetTextureSlice(), TextureSlice::Nine);
	}

	TEST_F(PatchTextureStd9T, ProcessRender)
	{
		m_component->ChangeOrigin(Origin::Center);
		m_component->ChangeSize({ 80, 80 }); //source ��ǥ�� �����ؼ� ����� �����ߴ�.

		//ȭ�鿡 �׷����� �κ�
		RECT destRightTop{ 18, -40, 40, -18 };
		RECT destLeftBottom{ -40, 18, -18, 40 };
		RECT destRightBottom{ 18, 18, 40, 40 };

		//�ؽ��Ŀ��� �������� ����
		RECT srcRightTop{ 26, 0, 48, 22 };
		RECT srcLeftBottom{ 0, 26, 22, 48 };
		RECT srcRightBottom{ 26, 26, 48, 48 };

		//9���� Render ȣ�����ٵ� �� �˻��ϴ°� �ƴ϶� 3������ �˻��Ѵ�.
		MockTextureRender render;
		EXPECT_CALL(render, Render).Times(::testing::AnyNumber());
		::testing::InSequence seq;
		EXPECT_CALL(render, Render(0, destRightTop, ::testing::Pointee(srcRightTop))).Times(1);
		EXPECT_CALL(render, Render(0, destLeftBottom, ::testing::Pointee(srcLeftBottom))).Times(1);
		EXPECT_CALL(render, Render(0, destRightBottom, ::testing::Pointee(srcRightBottom))).Times(1);

		m_component->UpdatePositionsManually();
		m_component->ProcessRender(&render);
	}

	TEST_F(PatchTextureStd9T, WriteAndRead)
	{
		EXPECT_TRUE(TestWriteAndRead(m_component, GetTempDir() + L"PatchTextureStd9T_WR.json"));
	}
}