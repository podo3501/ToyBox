#pragma once
#include "UserInterface/UIComponent/ComponentFixture.h"
#include "Toy/UserInterface/UIComponent/Components/PatchTexture/PatchTextureLite/PatchTextureLite9.h"

class PatchTextureLite9T : public ComponentFixture
{
protected:
	virtual void SetUp() override;
	virtual void RegisterRenderTextures(MockRenderer* renderer) override {}; //텍스쳐 정보 필요없음.
	virtual void RegisterBinderTextures(MockTextureResourceBinder* resBinder) override {}; //바인드 정보 필요없음.
	void InitializeBindSourceInfo();

	unique_ptr<PatchTextureLite9> m_component;
};

void PatchTextureLite9T::SetUp()
{
	ComponentFixture::SetUp();

	XMUINT2 size{ 48, 48 };
	m_component = CreateComponent<PatchTextureLite9>(size);
}

void PatchTextureLite9T::InitializeBindSourceInfo()
{
	vector<Rectangle> srcRects = {
		{  0,  0, 22, 22 }, { 22,  0,  4, 22 }, { 26,  0, 22, 22 }, // Top row
		{  0, 22, 22,  4 }, { 22, 22,  4,  4 }, { 26, 22, 22,  4 }, // Middle row
		{  0, 26, 22, 22 }, { 22, 26,  4, 22 }, { 26, 26, 22, 22 } // Bottom row
	};
	m_component->BindSourceInfo(0, srcRects);
}