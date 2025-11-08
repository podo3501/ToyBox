#include "pch.h"
#include "PatchTextureLite3T_Vertical.h"
#include "UserInterface/UIComponent/ComponentHelper.h"
#include "Shared/Utils/GeometryExt.h"

namespace UserInterfaceT::UIComponentT::ComponentT::PatchTextureT
{
	TEST_F(PatchTextureLite3T_Vertical, BindSourceInfo)
	{
		InitializeBindSourceInfo();

		auto centerComponent = m_component->GetCenterComponent();
		EXPECT_EQ(centerComponent->GetSize(), XMUINT2(48, 4)); //BindSourceInfo하면 source 사이즈값으로 맞춘다.
	}

	TEST_F(PatchTextureLite3T_Vertical, Clone)
	{
		EXPECT_TRUE(TestClone(m_component));
	}

	TEST_F(PatchTextureLite3T_Vertical, ChangeSize_Bigger)
	{
		InitializeBindSourceInfo();
		m_component->ChangeSize({ 80, 80 });

		auto centerComponent = m_component->GetCenterComponent();
		EXPECT_EQ(centerComponent->GetSize(), XMUINT2(80, 36)); // 80 - (22 * 2) = 36, 위쪽 아래쪽 사각형 x길이는 변하지 않는다.
	}

	//TEST_F(PatchTextureLite3T_Vertical, ChangeSize_Smaller) ?!?나중에 구현

	TEST_F(PatchTextureLite3T_Vertical, FitToTextureSource)
	{
		Rectangle srcTop{ 0, 0, 60, 20 };
		Rectangle srcMiddle{ 0, 20, 60, 20 };
		Rectangle srcBottom{ 0, 40, 60, 20 };

		m_component->SetIndexedSource(0, { srcTop, srcMiddle, srcBottom }); //SetIndexedSource는 BindSourceInfo 처럼 Fit 해주지 않는다.
		m_component->FitToTextureSource();

		EXPECT_EQ(m_component->GetSize(), XMUINT2(60, 60));
	}

	TEST_F(PatchTextureLite3T_Vertical, GetCenterComponent)
	{
		auto centerComponent = m_component->GetCenterComponent();
		EXPECT_EQ(centerComponent->GetArea(), Rectangle{});
	}

	TEST_F(PatchTextureLite3T_Vertical, GetTextureSlice)
	{
		EXPECT_EQ(m_component->GetTextureSlice(), TextureSlice::ThreeV);
	}

	TEST_F(PatchTextureLite3T_Vertical, ProcessRender)
	{
		InitializeBindSourceInfo();
		m_component->ChangeOrigin(Origin::Center);
		m_component->ChangeSize({ 80, 80 }); //source 좌표와 동일해서 사이즈를 조정했다.

		// 화면에 그려지는 부분
		RECT destTop{ -40, -40, 40, -18 };
		RECT destMiddle{ -40, -18, 40, 18 };
		RECT destBottom{ -40, 18, 40, 40 };

		// 텍스처에서 가져오는 영역
		RECT srcTop{ 0, 0, 48, 22 };
		RECT srcMiddle{ 0, 22, 48, 26 };
		RECT srcBottom{ 0, 26, 48, 48 };

		MockTextureRender render;
		::testing::InSequence seq;
		EXPECT_CALL(render, Render(0, destTop, ::testing::Pointee(srcTop))).Times(1);
		EXPECT_CALL(render, Render(0, destMiddle, ::testing::Pointee(srcMiddle))).Times(1);
		EXPECT_CALL(render, Render(0, destBottom, ::testing::Pointee(srcBottom))).Times(1);

		m_component->UpdatePositionsManually();
		m_component->ProcessRender(&render);
	}
}