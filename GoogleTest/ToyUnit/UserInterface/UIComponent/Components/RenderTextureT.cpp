#include "pch.h"
#include "RenderTextureT.h"
#include "../ComponentHelper.h"
#include "Shared/Utils/GeometryExt.h"
#include "Shared/Utils/StlExt.h"
#include "Toy/UserInterface/UIComponent/Traverser/UITraverser.h"

using namespace UITraverser;

namespace D::UserInterface::UIComponent::Component
{
	TEST_F(RenderTextureT, GetSize)
	{
		EXPECT_TRUE((m_component->GetSize() == XMUINT2{ 32, 32 })); //초기셋팅은 target component 의 기본 사이즈.
	}

	TEST_F(RenderTextureT, Clone)
	{
		EXPECT_TRUE(TestClone(m_component));
	}

	TEST_F(RenderTextureT, Clone_NewTexture) //내부적으로 사용하는 텍스쳐는 새로 만들어지는가?
	{
		auto texController = static_cast<MockTextureController*>(GetTextureController());
		EXPECT_CALL(*texController, CreateRenderTexture(testing::_, testing::_, testing::_)).Times(1);

		auto clone = Clone(m_component.get());
		BindTextureSourceInfo(clone.get(), GetResBinder()); //바인딩 할때 CreateRenderTexture를 한다.
	}

	TEST_F(RenderTextureT, Render)
	{
		ChangeSize(m_component.get(), { 100, 100 });

		size_t texIndex = 1; //새로 생성된 텍스쳐라 인덱스가 생성됨.
		RECT dest{ 0, 0, 100, 100 }; //사이즈 크기만큼.
		RECT source{ 0, 0, 100, 100 }; //텍스쳐를 사이즈 크기만큼 만들었기 때문에 값이 동일
		
		MockTextureRender render;
		EXPECT_CALL(render, Render(texIndex, dest, ::testing::Pointee(source))).Times(1);

		Render(m_component.get(), &render);
	}

	TEST_F(RenderTextureT, WriteAndRead)
	{
		EXPECT_TRUE(TestWriteAndRead(m_component, GetTempDir() + L"RenderTextureT_WR.json"));
	}
}