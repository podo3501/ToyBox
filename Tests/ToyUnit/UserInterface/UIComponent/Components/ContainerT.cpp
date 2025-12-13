#include "pch.h"
#include "ContainerT.h"

namespace D::UserInterface::UIComponent::Component
{
	TEST_F(ContainerT, Clone)
	{
		EXPECT_TRUE(TestClone(m_component));
	}

	TEST_F(ContainerT, Render)
	{
		RECT dest{ 0, 0, 30, 30 }; //CotainerT의 area가 아니라 Normal Component의 area이다.
		RECT source{ 0, 0, 30, 30 };

		MockTextureRender render;
		EXPECT_CALL(render, Render(0, dest, ::testing::Pointee(source)))
			.Times(1);
		
		Render(m_component.get(), &render);
	}

	TEST_F(ContainerT, WriteAndRead)
	{
		EXPECT_TRUE(TestWriteAndRead(m_component, GetTempDir() + L"ContainerT_WR.json", GetResBinder()));
	}
}