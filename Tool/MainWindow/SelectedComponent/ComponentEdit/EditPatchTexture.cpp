#include "pch.h"
#include "EditPatchTexture.h"
#include "../Include/IRenderer.h"
#include "../Toy/UserInterface/UIComponent/Components/PatchTexture/PatchTexture.h"
#include "../Toy/UserInterface/UIComponent/Components/PatchTexture/PatchTexture1.h"
#include "../Toy/UserInterface/UIComponent/Components/PatchTexture/PatchTexture3.h"
#include "../Toy/UserInterface/UIComponent/Components/PatchTexture/PatchTexture9.h"
#include "../Toy/UserInterface/Command/UICommandList/UICommandList.h"
#include "../Toy/UserInterface/TextureResourceBinder/TextureResourceBinder.h"
#include "../Toy/Utility.h"
#include "EditUtility/EditUtility.h"

EditPatchTexture::~EditPatchTexture() = default;
EditPatchTexture::EditPatchTexture(PatchTexture* patchTex, TextureResourceBinder* resBinder, UICommandList* cmdList) noexcept :
    EditWindow{ patchTex, resBinder, cmdList },
    m_patchTex{ patchTex }
{}

void EditPatchTexture::SetupComponent() noexcept
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

void EditPatchTexture::RenderComponent()
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

EditPatchTexture1::~EditPatchTexture1() = default;
EditPatchTexture1::EditPatchTexture1(PatchTexture1* patchTex1, IRenderer*, TextureResourceBinder* resBinder, UICommandList* cmdList) noexcept :
    EditPatchTexture{ patchTex1, resBinder, cmdList }
{}

////////////////////////////////////////////////

EditPatchTexture3::~EditPatchTexture3() = default;
EditPatchTexture3::EditPatchTexture3(PatchTexture3* patchTex3, IRenderer*, TextureResourceBinder* resBinder, UICommandList* cmdList) noexcept :
    EditPatchTexture{ patchTex3, resBinder, cmdList },
    m_patchTex3{ patchTex3 }
{}

////////////////////////////////////////////////

EditPatchTexture9::~EditPatchTexture9() = default;
EditPatchTexture9::EditPatchTexture9(PatchTexture9* patchTex9, IRenderer*, TextureResourceBinder* resBinder, UICommandList* cmdList) noexcept :
    EditPatchTexture{ patchTex9, resBinder, cmdList },
    m_patchTex9{ patchTex9 }
{}