#include "pch.h"
#include "EditPatchTextureLite.h"
#include "../Toy/UserInterface/UIComponent/Components/PatchTexture/PatchTextureLite/PatchTextureLite1.h"
#include "../Toy/UserInterface/UIComponent/Components/PatchTexture/PatchTextureLite/PatchTextureLite3.h"
#include "../Toy/UserInterface/UIComponent/Components/PatchTexture/PatchTextureLite/PatchTextureLite9.h"

EditPatchTextureLite::~EditPatchTextureLite() = default;
EditPatchTextureLite::EditPatchTextureLite(PatchTextureLite* patchTexL, UICommandList* cmdList) noexcept :
    EditPatchTexture{ patchTexL, cmdList },
    m_patchTexL{ patchTexL }
{
}

void EditPatchTextureLite::SetupComponent() noexcept
{
}

void EditPatchTextureLite::RenderComponent()
{
    EditPatchTexture::RenderComponent();
}

////////////////////////////////////////////////

EditPatchTextureLite1::~EditPatchTextureLite1() = default;
EditPatchTextureLite1::EditPatchTextureLite1(PatchTextureLite1* patchTexL1, UICommandList* cmdList) noexcept :
    EditPatchTextureLite{ patchTexL1, cmdList }
{
}

////////////////////////////////////////////////

EditPatchTextureLite3::~EditPatchTextureLite3() = default;
EditPatchTextureLite3::EditPatchTextureLite3(PatchTextureLite3* patchTexL3, UICommandList* cmdList) noexcept :
    EditPatchTextureLite{ patchTexL3, cmdList },
    m_patchTexL3{ patchTexL3 }
{
}

////////////////////////////////////////////////

EditPatchTextureLite9::~EditPatchTextureLite9() = default;
EditPatchTextureLite9::EditPatchTextureLite9(PatchTextureLite9* patchTexL9, UICommandList* cmdList) noexcept :
    EditPatchTextureLite{ patchTexL9, cmdList },
    m_patchTexL9{ patchTexL9 }
{
}