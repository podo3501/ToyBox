#include "pch.h"
#include "ContainerT.h"
#include "UserInterface/UIComponent/Component/ComponentHelper.h"
#include "Shared/Utils/GeometryExt.h"

namespace UserInterface::UIComponentT::ComponentT
{
	TEST_F(ContainerT, Clone)
	{
		EXPECT_TRUE(TestClone(m_component));
	}

	TEST_F(ContainerT, ProcessRender)
	{
		RECT dest{ 0, 0, 30, 30 }; //CotainerT�� area�� �ƴ϶� Normal Component�� area�̴�.
		RECT source{ 0, 0, 30, 30 };

		MockTextureRender render;
		EXPECT_CALL(render, Render(0, dest, ::testing::Pointee(source)))
			.Times(1);

		m_component->ProcessRender(&render);
	}

	TEST_F(ContainerT, WriteAndRead)
	{
		wstring filename = GetTempDir() + L"TextureSwitcherT_WR.json";
		EXPECT_TRUE(SerializerIO::WriteJsonToFile(m_component, filename));

		unique_ptr<Container> read;
		EXPECT_TRUE(SerializerIO::ReadJsonFromFile(filename, read));
		read->BindTextureSourceInfo(GetResBinder(), GetTextureController()); //���� �ε��ϸ鼭 PatchTextureLite ������ �Ϸ�ȴ�.

		EXPECT_TRUE(CompareDerived(m_component, read));
	}
}