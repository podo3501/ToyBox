#include "pch.h"
#include "PatchTextureStd9T.h"
#include "UserInterface/UIComponent/ComponentHelper.h"
#include "Shared/Utils/GeometryExt.h"
#include "Toy/UserInterface/UIComponent/Traverser/UITraverser.h"

using namespace UITraverser;

namespace UserInterfaceT::UIComponentT::ComponentT::PatchTextureT
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
		EXPECT_EQ(centerComponent->GetSize(), XMUINT2(36, 36)); // 80 - (22 * 2) = 36, 왼쪽 오른쪽 위 아래 사각형 x또는 y길이(22)는 변하지 않는다.
	}

	//?!? 작게 사이즈를 하면 버그가 생김
	//TEST_F(PatchTextureStd9T, ChangeSize_Smaller)
	//{
		//m_component->ChangeSize({ 30, 30 });
		//auto preSize = m_component->GetSize();
	//}

	TEST_F(PatchTextureStd9T, Clone)
	{
		EXPECT_TRUE(TestClone(m_component));
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
		m_component->ChangeSize({ 80, 80 }); //source 좌표와 동일해서 사이즈를 조정했다.

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

	TEST_F(PatchTextureStd9T, WriteAndRead)
	{
		EXPECT_TRUE(TestWriteAndRead(m_component, GetTempDir() + L"PatchTextureStd9T_WR.json"));
	}
}