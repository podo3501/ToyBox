#pragma once
#include "UserInterface/UIComponent/Components/ComponentT.h"
#include "Toy/UserInterface/UIComponent/Components/PatchTexture/PatchTextureLite/PatchTextureLite1.h"

class PatchTextureLite1T : public ComponentT
{
protected:
	virtual void SetUp() override;
	virtual void TearDown() override {};
	virtual void RegisterRenderTextures(MockRenderer* renderer) override {}; //�ؽ��� ���� �ʿ����.
	virtual void RegisterBinderTextures(MockTextureResourceBinder* resBinder) override {}; //���ε� ���� �ʿ����.

	unique_ptr<PatchTextureLite1> m_component;
};

void PatchTextureLite1T::SetUp()
{
	ComponentT::SetUp();

	XMUINT2 size{ 20, 20 };
	m_component = CreateComponent<PatchTextureLite1>(size);
}

