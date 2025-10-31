#pragma once
#include "UserInterface/UIComponent/Components/ComponentT.h"
#include "Toy/UserInterface/UIComponent/Components/PatchTexture/PatchTextureStd/PatchTextureStd9.h"

class PatchTextureStd9T : public ComponentT
{
protected:
	virtual void SetUp() override;
	virtual void TearDown() override {};
	virtual void RegisterBinderTextures(MockTextureResourceBinder* resBinder) override;

	unique_ptr<PatchTextureStd9> m_component;
};

///////////////////////////////////////////////////////////////

void PatchTextureStd9T::SetUp()
{
	ComponentT::SetUp();

	m_component = CreateComponent<PatchTextureStd9>("Nine48");
	m_component->BindTextureSourceInfo(GetResBinder(), GetTextureController());
}

void PatchTextureStd9T::RegisterBinderTextures(MockTextureResourceBinder* resBinder)
{
	resBinder->AddMockTextureKeys({
		{ "Nine48", { L"Texture512.png", TextureSlice::Nine, {	//20, 8, 20 인 정사각형 9방향 좌표
			{ 0, 0, 22, 22 }, { 22, 0, 4, 22 }, { 26, 0, 22, 22 },
			{ 0, 22, 22, 4 }, { 22, 22, 4, 4 }, { 26, 22, 22, 4 },
			{ 0, 26, 22, 22 }, { 22, 26, 4, 22 }, { 26, 26, 22, 22 } } } },
		{ "Nine60", { L"Texture512.png", TextureSlice::Nine, {	//25, 10, 25 인 정사각형 9방향 좌표
			{ 0, 0, 25, 25 }, { 25, 0, 10, 25 }, { 35, 0, 25, 25 },
			{ 0, 25, 25, 10 }, { 25, 25, 10, 10 }, { 35, 25, 25, 10 },
			{ 0, 35, 25, 25 }, { 25, 35, 10, 25 }, { 35, 35, 25, 25 } } } }
		});
}