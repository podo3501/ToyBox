#pragma once
#include "UserInterface/UIComponent/Components/ComponentT.h"
#include "Toy/UserInterface/UIComponent/Components/PatchTexture/PatchTextureLite/PatchTextureLite1.h"

class PatchTextureLite1T : public ComponentT
{
protected:
	virtual void SetUp() override;
	virtual void TearDown() override {};
	virtual void RegisterRenderTextures(MockRenderer* renderer) override {}; //텍스쳐 정보 필요없음.
	virtual void RegisterBinderTextures(MockTextureResourceBinder* resBinder) override {}; //바인드 정보 필요없음.

	unique_ptr<PatchTextureLite1> m_component;
};

void PatchTextureLite1T::SetUp()
{
	ComponentT::SetUp();

	XMUINT2 size{ 20, 20 };
	m_component = CreateComponent<PatchTextureLite1>(size);
}

