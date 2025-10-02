#include "pch.h"
#include "EditPatchTexture.h"
#include "Toy/UserInterface/UIComponent/Components/PatchTexture/PatchTexture.h"
#include "Toy/UserInterface/Command/UICommandList/UICommandList.h"

EditPatchTexture::~EditPatchTexture() = default;
EditPatchTexture::EditPatchTexture(PatchTexture* patchTex, UICommandList* cmdList) noexcept :
	EditWindow{ patchTex, cmdList },
	m_patchTex{ patchTex }
{}

void EditPatchTexture::RenderComponent()
{
	if (ImGui::Button("Fit to Texture Size"))
	{
		auto cmdList = GetUICommandList();
		cmdList->FitToTextureSource(m_patchTex);
	}
}