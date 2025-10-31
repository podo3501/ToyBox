#include "pch.h"
#include "PatchTextureStd1T.h"
#include "UserInterface/UIComponent/Components/ComponentHelper.h"
#include "Shared/Utils/GeometryExt.h"
#include "Shared/Utils/StlExt.h"

namespace UserInterfaceT::UIComponentT::ComponentT::PatchTextureT
{
	TEST_F(PatchTextureStd1T, ChangeBindKey)
	{
		m_component->ChangeBindKey(GetResBinder(), "One64");
		EXPECT_EQ(m_component->GetArea(), Rectangle(0, 0, 64, 64));
	}

	TEST_F(PatchTextureStd1T, Clone)
	{
		EXPECT_TRUE(TestClone(m_component));
	}

	TEST_F(PatchTextureStd1T, FitToTextureSource)
	{
		auto preSize = m_component->GetSize();
		m_component->ChangeSize({ 10, 10 });
		
		m_component->FitToTextureSource();
		EXPECT_EQ(m_component->GetSize(), preSize);
	}

	TEST_F(PatchTextureStd1T, GetCenterComponent)
	{
		auto centerComponent = m_component->GetCenterComponent();
		EXPECT_EQ(centerComponent, m_component.get());
	}

	TEST_F(PatchTextureStd1T, GetTextureSlice)
	{
		EXPECT_EQ(m_component->GetTextureSlice(), TextureSlice::One);
	}

	TEST_F(PatchTextureStd1T, ProcessRender)
	{
		RECT dest = RectangleToRect(m_component->GetArea());
		RECT source = RectangleToRect(m_component->GetSource());

		MockTextureRender render;
		EXPECT_CALL(render, Render(0, dest, ::testing::Pointee(source)))
			.Times(1);

		m_component->ProcessRender(&render);
	}

	TEST_F(PatchTextureStd1T, WriteAndRead)
	{
		EXPECT_TRUE(TestWriteAndRead(m_component, GetTempDir() + L"PatchtextureStd1T_WR.json"));
	}
}