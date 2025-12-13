#include "pch.h"
#include "PatchTextureLite1T.h"

namespace D::UserInterface::UIComponent::Component::PatchTexture
{
	TEST_F(PatchTextureLite1T, BindSourceInfo)
	{
		//SetIndexedSource와 다른점은 Bind는 바로 쓰는 느낌이고, SetIndexedSource는 정보만 일단 넣는느낌.
		//3과 9일때 차이가 난다. 그리고 크기가 없다면 기본 크기(source크기)를 셋팅해 준다.
		Rectangle source{ 0, 0, 10, 10 };
		m_component->BindSourceInfo(0, { source });
		EXPECT_EQ(m_component->GetSource(), source);
	}

	TEST_F(PatchTextureLite1T, Clone)
	{
		EXPECT_TRUE(TestClone(m_component));
	}

	TEST_F(PatchTextureLite1T, FitToTextureSource)
	{
		m_component->SetIndexedSource(0, { { 0, 0, 10, 10 } }); //처음에는 텍스쳐의 정보가 없다.
		m_component->FitToTextureSource();

		EXPECT_EQ(m_component->GetSize(), XMUINT2(10, 10));
	}

	TEST_F(PatchTextureLite1T, GetCenterComponent)
	{
		auto centerComponent = m_component->GetCenterComponent();
		EXPECT_EQ(centerComponent, m_component.get());
	}

	TEST_F(PatchTextureLite1T, GetTextureSlice)
	{
		EXPECT_EQ(m_component->GetTextureSlice(), TextureSlice::One);
	}

	TEST_F(PatchTextureLite1T, Render)
	{
		RECT dest{ 0, 0, 20, 20 };
		RECT source{ 0, 0, 10, 10 };
		ChangeSize(m_component.get(), { 20, 20 }); //dest의 크기
		m_component->BindSourceInfo(0, { { 0, 0, 10, 10 } }); //texture index, source의 좌표

		MockTextureRender render;
		EXPECT_CALL(render, Render(0, dest, ::testing::Pointee(source)))
			.Times(1);

		Render(m_component.get(), &render);
	}
}