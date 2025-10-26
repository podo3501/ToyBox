#include "pch.h"
#include "PatchTextureStd3T_Horizontal.h"
#include "Shared/SerializerIO/SerializerIO.h"
#include "Shared/Utils/GeometryExt.h"

namespace UserInterface::UIComponentT::ComponentT
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
		auto clone = m_component->Clone();
		EXPECT_TRUE(CompareDerived(m_component, clone));
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

	TEST_F(PatchTextureStd3T_Horizontal, WriteAndRead)
	{
		wstring filename = L"../Resources/Test/Temp/PatchtextureStd3T_Horizontal_WriteAndRead.json";
		EXPECT_TRUE(SerializerIO::WriteJsonToFile(m_component, filename));

		unique_ptr<PatchTextureStd3> read;
		EXPECT_TRUE(SerializerIO::ReadJsonFromFile(filename, read));

		EXPECT_TRUE(CompareDerived(m_component, read));
	}
}