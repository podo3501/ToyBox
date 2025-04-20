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
EditPatchTextureStd::EditPatchTextureStd(PatchTextureStd* patchTex, UICommandList* cmdList, TextureResourceBinder* resBinder) noexcept :
    EditPatchTexture{ patchTex, cmdList },
    m_resBinder{ resBinder },
    m_patchTex{ patchTex }
{}

void EditPatchTextureStd::SetupComponent() noexcept
{
    auto& curKey = m_patchTex->GetBindKey();
    if (curKey.empty()) return;

    if (auto opSlice = m_patchTex->GetTextureSlice(); opSlice)
    {
        const auto& keys = m_resBinder->GetTextureKeys(*opSlice);
        if (ranges::find(keys, curKey) == keys.end()) return;

        m_combo = make_unique<EditCombo>("Bind Keys", keys);
        m_combo->SelectItem(curKey);
    }
}

void EditPatchTextureStd::RenderComponent()
{
    EditPatchTexture::RenderComponent();

    if (!m_combo) return;
    m_combo->Render([this](const string& curKey) {
        if (auto infoRef = m_resBinder->GetTextureSourceInfo(curKey); infoRef)
            m_patchTex->ChangeBindKey(curKey, *infoRef);
        });
}

////////////////////////////////////////////////

EditPatchTextureStd1::~EditPatchTextureStd1() = default;
EditPatchTextureStd1::EditPatchTextureStd1(PatchTextureStd1* patchTex1, UICommandList* cmdList, TextureResourceBinder* resBinder) noexcept :
    EditPatchTextureStd{ patchTex1, cmdList, resBinder}
{}

////////////////////////////////////////////////

EditPatchTextureStd3::~EditPatchTextureStd3() = default;
EditPatchTextureStd3::EditPatchTextureStd3(PatchTextureStd3* patchTex3, UICommandList* cmdList, TextureResourceBinder* resBinder) noexcept :
    EditPatchTextureStd{ patchTex3, cmdList, resBinder },
    m_patchTex3{ patchTex3 }
{}

////////////////////////////////////////////////

EditPatchTextureStd9::~EditPatchTextureStd9() = default;
EditPatchTextureStd9::EditPatchTextureStd9(PatchTextureStd9* patchTex9, UICommandList* cmdList, TextureResourceBinder* resBinder) noexcept :
    EditPatchTextureStd{ patchTex9, cmdList, resBinder },
    m_patchTex9{ patchTex9 }
{}