#include "pch.h"
#include "PatchTextureStd3T_Horizontal.h"
#include "UserInterface/UIComponent/ComponentHelper.h"
#include "Shared/Utils/GeometryExt.h"
#include "Toy/UserInterface/UIComponent/Traverser/UITraverser.h"

using namespace UITraverser;

namespace D::UserInterface::UIComponent::Component::PatchTexture
{
	TEST_F(PatchTextureStd3T_Horizontal, ChangeBindKey)
	{
		m_component->ChangeBindKey(GetResBinder(), "ThreeH60");
		EXPECT_EQ(m_component->GetArea(), Rectangle(0, 0, 60, 60));
	}

	TEST_F(PatchTextureStd3T_Horizontal, ChangeSize_Bigger)
	{
		ChangeSize(m_component.get(), { 80, 80 });
		auto centerComponent = m_component->GetCenterComponent();
		EXPECT_EQ(centerComponent->GetSize(), XMUINT2(36, 80)); // 80 - (22 * 2) = 36, 왼쪽 오른쪽 사각형 x길이는 변하지 않는다.
	}

	//?!? 작게 사이즈를 하면 버그가 생김
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
		ChangeSize(m_component.get(), { 60, 60 });

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

	TEST_F(PatchTextureStd3T_Horizontal, Render)
	{
		m_component->ChangeOrigin(Origin::Center);
		ChangeSize(m_component.get(), { 80, 80 }); //source 좌표와 동일해서 사이즈를 조정했다.

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

		UpdatePositionsManually(m_component.get());
		Render(m_component.get(), &render);
	}

	TEST_F(PatchTextureStd3T_Horizontal, WriteAndRead)
	{
		EXPECT_TRUE(TestWriteAndRead(m_component, GetTempDir() + L"PatchTextureStd3T_Horizontal_WR.json"));
	}
}