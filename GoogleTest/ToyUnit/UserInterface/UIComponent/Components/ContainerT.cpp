#include "pch.h"
#include "ContainerT.h"
#include "../ComponentHelper.h"
#include "Shared/Utils/GeometryExt.h"
#include "Toy/UserInterface/UIComponent/Traverser/UITraverser.h"

namespace UserInterfaceT::UIComponentT::ComponentT
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

		UITraverser::Render(m_component.get(), &render);
	}

	TEST_F(ContainerT, WriteAndRead)
	{
		wstring filename = GetTempDir() + L"TextureSwitcherT_WR.json";
		EXPECT_TRUE(SerializerIO::WriteJsonToFile(m_component, filename));

		unique_ptr<Container> read;
		EXPECT_TRUE(SerializerIO::ReadJsonFromFile(filename, read));
		BindTextureSourceInfo(read.get(), GetResBinder()); //실제 로드하면서 PatchTextureLite 설정이 완료된다.

		EXPECT_TRUE(CompareDerived(m_component, read));
	}
}