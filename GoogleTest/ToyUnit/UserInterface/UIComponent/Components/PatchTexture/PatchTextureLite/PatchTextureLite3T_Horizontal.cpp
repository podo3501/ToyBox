#include "pch.h"
#include "PatchTextureLite3T_Horizontal.h"
#include "UserInterface/UIComponent/Components/ComponentHelper.h"
#include "Shared/Utils/GeometryExt.h"

namespace UserInterfaceT::UIComponentT::ComponentT::PatchTextureT
{
	TEST_F(PatchTextureLite3T_Horizontal, BindSourceInfo)
	{
		InitializeBindSourceInfo();

		auto centerComponent = m_component->GetCenterComponent();
		EXPECT_EQ(centerComponent->GetSize(), XMUINT2(4, 48)); //BindSourceInfo하면 source 사이즈값으로 맞춘다.
	}

	TEST_F(PatchTextureLite3T_Horizontal, Clone)
	{
		EXPECT_TRUE(TestClone(m_component));
	}

	TEST_F(PatchTextureLite3T_Horizontal, ChangeSize_Bigger)
	{
		InitializeBindSourceInfo();
		m_component->ChangeSize({ 80, 80 });

		auto centerComponent = m_component->GetCenterComponent();
		EXPECT_EQ(centerComponent->GetSize(), XMUINT2(36, 80)); // 80 - (22 * 2) = 36, 왼쪽 오른쪽 사각형 x길이는 변하지 않는다.
	}

	//TEST_F(PatchTextureLite3T_Horizontal, ChangeSize_Smaller) ?!?나중에 구현

	TEST_F(PatchTextureLite3T_Horizontal, FitToTextureSource)
	{
		Rectangle srcLeft{ 0, 0, 20, 60 };
		Rectangle srcCenter{ 20, 0, 20, 60 };
		Rectangle srcRight{ 40, 0, 20, 60 };

		m_component->SetIndexedSource(0, { srcLeft, srcCenter, srcRight }); //SetIndexedSource는 BindSourceInfo 처럼 Fit 해주지 않는다.
		m_component->FitToTextureSource();

		EXPECT_EQ(m_component->GetSize(), XMUINT2(60, 60));
	}

	TEST_F(PatchTextureLite3T_Horizontal, GetCenterComponent)
	{
		auto centerComponent = m_component->GetCenterComponent();
		EXPECT_EQ(centerComponent->GetArea(), Rectangle{});
	}

	TEST_F(PatchTextureLite3T_Horizontal, GetTextureSlice)
	{
		EXPECT_EQ(m_component->GetTextureSlice(), TextureSlice::ThreeH);
	}

	TEST_F(PatchTextureLite3T_Horizontal, ProcessRender)
	{
		InitializeBindSourceInfo();
		m_component->ChangeOrigin(Origin::Center);
		m_component->ChangeSize({ 80, 80 }); //source 좌표와 동일해서 사이즈를 조정했다.

		//화면에 그려지는 부분
		RECT destLeft{ -40, -40, -18, 40 };
		RECT destCenter{ -18, -40, 18, 40 };
		RECT destRight{ 18, -40, 40, 40 };

		//텍스쳐에서 가져오는 영역
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
}