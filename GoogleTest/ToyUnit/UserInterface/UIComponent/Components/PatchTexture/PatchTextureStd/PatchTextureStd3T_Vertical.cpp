#include "pch.h"
#include "PatchTextureStd3T_Vertical.h"
#include "UserInterface/UIComponent/Components/ComponentHelper.h"
#include "Shared/Utils/GeometryExt.h"

namespace UserInterfaceT::UIComponentT::ComponentT::PatchTextureT
{
	TEST_F(PatchTextureStd3T_Vertical, ChangeSize_Bigger)
	{
		m_component->ChangeSize({ 80, 80 });
		auto centerComponent = m_component->GetCenterComponent();
		EXPECT_EQ(centerComponent->GetSize(), XMUINT2(80, 36)); // 80 - (22 * 2) = 36, 위쪽 아래쪽 사각형 y길이는 변하지 않는다.
	}

	//?!? 작게 사이즈를 하면 버그가 생김
	//TEST_F(PatchTextureStd3T_Vertical, ChangeSize_Smaller)
	//{
		//m_component->ChangeSize({ 30, 30 });
		//auto preSize = m_component->GetSize();
	//}

	TEST_F(PatchTextureStd3T_Vertical, Clone)
	{
		EXPECT_TRUE(TestClone(m_component));
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
		m_component->ChangeSize({ 80, 80 }); //source 좌표와 동일해서 사이즈를 조정했다.

		//화면에 그려지는 부분
		RECT destTop{ -40, -40,  40, -18 };
		RECT destMiddle{ -40, -18,  40,  18 };
		RECT destBottom{ -40,  18,  40,  40 };

		//텍스쳐에서 가져오는 영역
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
		EXPECT_TRUE(TestWriteAndRead(m_component, GetTempDir() + L"PatchTextureStd3T_Vertical_WR.json"));
	}
}