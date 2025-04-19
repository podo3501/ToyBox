#include "pch.h"
#include "EditPatchTextureStd.h"
#include "../Include/IRenderer.h"
#include "../Toy/UserInterface/UIComponent/Components/PatchTexture/PatchTextureStd/PatchTextureStd1.h"
#include "../Toy/UserInterface/UIComponent/Components/PatchTexture/PatchTextureStd/PatchTextureStd3.h"
#include "../Toy/UserInterface/UIComponent/Components/PatchTexture/PatchTextureStd/PatchTextureStd9.h"
#include "../Toy/UserInterface/Command/UICommandList/UICommandList.h"
#include "../Toy/UserInterface/TextureResourceBinder/TextureResourceBinder.h"
#include "../Toy/Utility.h"
#include "EditUtility/EditUtility.h"

EditPatchTextureStd::~EditPatchTextureStd() = default;
EditPatchTextureStd::EditPatchTextureStd(PatchTextureStd* patchTex, TextureResourceBinder* resBinder, UICommandList* cmdList) noexcept :
    EditWindow{ patchTex, resBinder, cmdList },
    m_patchTex{ patchTex }
{}

void EditPatchTextureStd::SetupComponent() noexcept
{
    auto& curKey = m_patchTex->GetBindKey();
    if (curKey.empty()) return;

    auto resBinder = GetTextureResourceBinder();
    if (auto opSlice = m_patchTex->GetTextureSlice(); opSlice)
    {
        const auto& keys = resBinder->GetTextureKeys(*opSlice);
        if (ranges::find(keys, curKey) == keys.end()) return;

        m_combo = make_unique<EditCombo>("Bind Keys", keys);
        m_combo->SelectItem(curKey);
    }
}

void EditPatchTextureStd::RenderComponent()
{
    if (!m_combo) return;

    if (ImGui::Button("Fit to Texture Size"))
        m_patchTex->FitToTextureSource();

    auto resBinder = GetTextureResourceBinder();
    m_combo->Render([this, &resBinder](const string& curKey) {
        if (auto infoRef = resBinder->GetTextureSourceInfo(curKey); infoRef)
            m_patchTex->ChangeBindKey(curKey, *infoRef);
        });
}

////////////////////////////////////////////////

EditPatchTextureStd1::~EditPatchTextureStd1() = default;
EditPatchTextureStd1::EditPatchTextureStd1(PatchTextureStd1* patchTex1, IRenderer*, TextureResourceBinder* resBinder, UICommandList* cmdList) noexcept :
    EditPatchTextureStd{ patchTex1, resBinder, cmdList }
{}

////////////////////////////////////////////////

EditPatchTextureStd3::~EditPatchTextureStd3() = default;
EditPatchTextureStd3::EditPatchTextureStd3(PatchTextureStd3* patchTex3, IRenderer*, TextureResourceBinder* resBinder, UICommandList* cmdList) noexcept :
    EditPatchTextureStd{ patchTex3, resBinder, cmdList },
    m_patchTex3{ patchTex3 }
{}

////////////////////////////////////////////////

EditPatchTextureStd9::~EditPatchTextureStd9() = default;
EditPatchTextureStd9::EditPatchTextureStd9(PatchTextureStd9* patchTex9, IRenderer*, TextureResourceBinder* resBinder, UICommandList* cmdList) noexcept :
    EditPatchTextureStd{ patchTex9, resBinder, cmdList },
    m_patchTex9{ patchTex9 }
{}