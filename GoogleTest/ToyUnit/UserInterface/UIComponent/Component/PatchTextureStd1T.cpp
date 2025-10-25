#include "pch.h"
#include "PatchTextureStd1T.h"
#include "Shared/Utils/GeometryExt.h"
#include "Shared/Utils/StlExt.h"
#include "Shared/SerializerIO/SerializerIO.h"

namespace UserInterface::UIComponentT::ComponentT
{
	TEST_F(PatchTextureStd1T, ChangeBindKey)
	{
		m_component->ChangeBindKey(m_resBinder.get(), "One64");
		EXPECT_EQ(m_component->GetArea(), Rectangle(0, 0, 64, 64));
	}

	TEST_F(PatchTextureStd1T, Clone)
	{
		auto clone = m_component->Clone();
		EXPECT_TRUE(CompareDerived(m_component, clone));
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
		wstring filename = L"../Resources/Test/Temp/PatchtextureStd1T_WriteAndRead.json";
		EXPECT_TRUE(SerializerIO::WriteJsonToFile(m_component, filename));

		unique_ptr<PatchTextureStd1> read;
		EXPECT_TRUE(SerializerIO::ReadJsonFromFile(filename, read));

		EXPECT_TRUE(CompareDerived(m_component, read));
	}
}