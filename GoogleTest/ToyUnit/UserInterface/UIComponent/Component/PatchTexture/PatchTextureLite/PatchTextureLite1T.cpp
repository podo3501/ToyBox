#include "pch.h"
#include "PatchTextureLite1T.h"
#include "UserInterface/UIComponent/Component/ComponentHelper.h"
#include "Shared/Utils/GeometryExt.h"

namespace UserInterface::UIComponentT::ComponentT::PatchTextureT
{
	TEST_F(PatchTextureLite1T, BindSourceInfo)
	{
		//SetIndexedSource�� �ٸ����� Bind�� �ٷ� ���� �����̰�, SetIndexedSource�� ������ �ϴ� �ִ´���.
		//3�� 9�϶� ���̰� ����. �׸��� ũ�Ⱑ ���ٸ� �⺻ ũ��(sourceũ��)�� ������ �ش�.
		//?!?SetIndextedSource�� ����ؼ� �����丵 �ؾ��ҵ�
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
		m_component->SetIndexedSource(0, { { 0, 0, 10, 10 } }); //ó������ �ؽ����� ������ ����.
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

	TEST_F(PatchTextureLite1T, ProcessRender)
	{
		RECT dest{ 0, 0, 20, 20 };
		RECT source{ 0, 0, 10, 10 };
		m_component->ChangeSize({ 20, 20 }); //dest�� ũ��
		m_component->BindSourceInfo(0, { { 0, 0, 10, 10 } }); //texture index, source�� ��ǥ

		MockTextureRender render;
		EXPECT_CALL(render, Render(0, dest, ::testing::Pointee(source)))
			.Times(1);

		m_component->ProcessRender(&render);
	}
}