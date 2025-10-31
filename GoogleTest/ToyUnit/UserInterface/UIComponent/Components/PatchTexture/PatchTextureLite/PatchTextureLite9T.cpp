#include "pch.h"
#include "PatchTextureLite9T.h"
#include "UserInterface/UIComponent/Components/ComponentHelper.h"
#include "Shared/Utils/GeometryExt.h"

namespace UserInterfaceT::UIComponentT::ComponentT::PatchTextureT
{
	TEST_F(PatchTextureLite9T, BindSourceInfo)
	{
		InitializeBindSourceInfo();

		auto centerComponent = m_component->GetCenterComponent();
		EXPECT_EQ(centerComponent->GetSize(), XMUINT2(4, 4)); //BindSourceInfo�ϸ� source ��������� �����.
	}

	TEST_F(PatchTextureLite9T, Clone)
	{
		EXPECT_TRUE(TestClone(m_component));
	}

	TEST_F(PatchTextureLite9T, ChangeSize_Bigger)
	{
		InitializeBindSourceInfo();
		m_component->ChangeSize({ 80, 80 });

		auto centerComponent = m_component->GetCenterComponent();
		EXPECT_EQ(centerComponent->GetSize(), XMUINT2(36, 36)); // 80 - (22 * 2) = 36, ���� ������ �簢�� x���̴� ������ �ʴ´�.
	}

	//TEST_F(PatchTextureLite3T_Horizontal, ChangeSize_Smaller) ?!?���߿� ����

	TEST_F(PatchTextureLite9T, FitToTextureSource)
	{
		vector<Rectangle> srcRects = {
			{  0,  0, 20, 20 }, { 20,  0, 20, 20 }, { 40,  0, 20, 20 }, // Top row
			{  0, 20, 20, 20 }, { 20, 20, 20, 20 }, { 40, 20, 20, 20 }, // Middle row
			{  0, 40, 20, 20 }, { 20, 40, 20, 20 }, { 40, 40, 20, 20 } // Bottom row
		};

		m_component->SetIndexedSource(0, srcRects); //SetIndexedSource�� BindSourceInfo ó�� Fit ������ �ʴ´�.
		m_component->FitToTextureSource();

		EXPECT_EQ(m_component->GetSize(), XMUINT2(60, 60));
	}

	TEST_F(PatchTextureLite9T, GetCenterComponent)
	{
		auto centerComponent = m_component->GetCenterComponent();
		EXPECT_EQ(centerComponent->GetArea(), Rectangle{});
	}

	TEST_F(PatchTextureLite9T, GetTextureSlice)
	{
		EXPECT_EQ(m_component->GetTextureSlice(), TextureSlice::Nine);
	}

	TEST_F(PatchTextureLite9T, ProcessRender)
	{
		InitializeBindSourceInfo();
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
}