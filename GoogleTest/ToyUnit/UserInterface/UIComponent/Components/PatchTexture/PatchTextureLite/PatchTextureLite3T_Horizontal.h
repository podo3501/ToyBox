#pragma once
#include "UserInterface/UIComponent/Components/ComponentT.h"
#include "Toy/UserInterface/UIComponent/Components/PatchTexture/PatchTextureLite/PatchTextureLite3.h"

class PatchTextureLite3T_Horizontal : public ComponentT
{
protected:
	virtual void SetUp() override;
	virtual void TearDown() override {};
	virtual void RegisterRenderTextures(MockRenderer* renderer) override {}; //�ؽ��� ���� �ʿ����.
	virtual void RegisterBinderTextures(MockTextureResourceBinder* resBinder) override {}; //���ε� ���� �ʿ����.
	void InitializeBindSourceInfo();

	unique_ptr<PatchTextureLite3> m_component;
};

void PatchTextureLite3T_Horizontal::SetUp()
{
	ComponentT::SetUp();

	XMUINT2 size{ 48, 48 };
	m_component = CreateComponent<PatchTextureLite3>(size, DirectionType::Horizontal);
}

void PatchTextureLite3T_Horizontal::InitializeBindSourceInfo()
{
	Rectangle srcLeft{ 0, 0, 22, 48 };
	Rectangle srcCenter{ 22, 0, 4, 48 };
	Rectangle srcRight{ 26, 0, 22, 48 };
	m_component->BindSourceInfo(0, { srcLeft, srcCenter, srcRight });
}