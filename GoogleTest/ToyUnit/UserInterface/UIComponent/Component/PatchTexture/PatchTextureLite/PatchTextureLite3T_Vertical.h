#pragma once
#include "UserInterface/UIComponent/Component/ComponentT.h"
#include "Toy/UserInterface/UIComponent/Components/PatchTexture/PatchTextureLite/PatchTextureLite3.h"

class PatchTextureLite3T_Vertical : public ComponentT
{
protected:
	virtual void SetUp() override;
	virtual void TearDown() override {};
	virtual void RegisterRenderTextures(MockRenderer* renderer) override {}; //�ؽ��� ���� �ʿ����.
	virtual void RegisterBinderTextures(MockTextureResourceBinder* resBinder) override {}; //���ε� ���� �ʿ����.
	void InitializeBindSourceInfo();

	unique_ptr<PatchTextureLite3> m_component;
};

void PatchTextureLite3T_Vertical::SetUp()
{
	ComponentT::SetUp();

	XMUINT2 size{ 48, 48 };
	m_component = CreateComponent<PatchTextureLite3>(size, DirectionType::Vertical);
}

void PatchTextureLite3T_Vertical::InitializeBindSourceInfo()
{
	Rectangle srcTop{ 0, 0, 48, 22 };
	Rectangle srcMiddle{ 0, 22, 48, 4 };
	Rectangle srcBottom{ 0, 26, 48, 22 };
	m_component->BindSourceInfo(0, { srcTop, srcMiddle, srcBottom });
}