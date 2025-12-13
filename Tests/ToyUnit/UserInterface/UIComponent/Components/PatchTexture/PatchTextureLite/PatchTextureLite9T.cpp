#include "pch.h"
#include "PatchTextureLite9T.h"

namespace D::UserInterface::UIComponent::Component::PatchTexture
{
	TEST_F(PatchTextureLite9T, BindSourceInfo)
	{
		InitializeBindSourceInfo();

		auto centerComponent = m_component->GetCenterComponent();
		EXPECT_EQ(centerComponent->GetSize(), XMUINT2(4, 4)); //BindSourceInfo하면 source 사이즈값으로 맞춘다.
	}

	TEST_F(PatchTextureLite9T, Clone)
	{
		EXPECT_TRUE(TestClone(m_component));
	}

	TEST_F(PatchTextureLite9T, ChangeSize_Bigger)
	{
		InitializeBindSourceInfo();
		ChangeSize(m_component.get(), { 80, 80 });

		auto centerComponent = m_component->GetCenterComponent();
		EXPECT_EQ(centerComponent->GetSize(), XMUINT2(36, 36)); // 80 - (22 * 2) = 36, 왼쪽 오른쪽 사각형 x길이는 변하지 않는다.
	}

	//TEST_F(PatchTextureLite3T_Horizontal, ChangeSize_Smaller) ?!?나중에 구현

	TEST_F(PatchTextureLite9T, FitToTextureSource)
	{
		vector<Rectangle> srcRects = {
			{  0,  0, 20, 20 }, { 20,  0, 20, 20 }, { 40,  0, 20, 20 }, // Top row
			{  0, 20, 20, 20 }, { 20, 20, 20, 20 }, { 40, 20, 20, 20 }, // Middle row
			{  0, 40, 20, 20 }, { 20, 40, 20, 20 }, { 40, 40, 20, 20 } // Bottom row
		};

		m_component->SetIndexedSource(0, srcRects); //SetIndexedSource는 BindSourceInfo 처럼 Fit 해주지 않는다.
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

	TEST_F(PatchTextureLite9T, Render)
	{
		InitializeBindSourceInfo();
		m_component->ChangeOrigin(Origin::Center);
		ChangeSize(m_component.get(), { 80, 80 }); //source 좌표와 동일해서 사이즈를 조정했다.

		//화면에 그려지는 부분
		RECT destRightTop{ 18, -40, 40, -18 };
		RECT destLeftBottom{ -40, 18, -18, 40 };
		RECT destRightBottom{ 18, 18, 40, 40 };

		//텍스쳐에서 가져오는 영역
		RECT srcRightTop{ 26, 0, 48, 22 };
		RECT srcLeftBottom{ 0, 26, 22, 48 };
		RECT srcRightBottom{ 26, 26, 48, 48 };

		//9번을 Render 호출할텐데 다 검사하는게 아니라 3군데만 검사한다.
		MockTextureRender render;
		EXPECT_CALL(render, Render).Times(::testing::AnyNumber());
		::testing::InSequence seq;
		EXPECT_CALL(render, Render(0, destRightTop, ::testing::Pointee(srcRightTop))).Times(1);
		EXPECT_CALL(render, Render(0, destLeftBottom, ::testing::Pointee(srcLeftBottom))).Times(1);
		EXPECT_CALL(render, Render(0, destRightBottom, ::testing::Pointee(srcRightBottom))).Times(1);

		UpdatePositionsManually(m_component.get());
		Render(m_component.get(), &render);
	}
}