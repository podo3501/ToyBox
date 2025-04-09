#include "pch.h"
#include "EditPatchTexture.h"
#include "../Include/IRenderer.h"
#include "../Toy/UserInterface/UIComponent/Components/PatchTexture/PatchTexture1.h"
#include "../Toy/UserInterface/UIComponent/Components/PatchTexture/PatchTexture3.h"
#include "../Toy/UserInterface/UIComponent/Components/PatchTexture/PatchTexture9.h"
#include "../Toy/UserInterface/Command/UICommandList/UICommandList.h"
#include "../Toy/UserInterface/TextureResourceBinder/TextureResourceBinder.h"
#include "../Toy/Utility.h"
#include "EditUtility/EditUtility.h"

EditPatchTexture1::~EditPatchTexture1() = default;
EditPatchTexture1::EditPatchTexture1(PatchTexture1* patchTex1, IRenderer*, TextureResourceBinder* resBinder, UICommandList* cmdList) noexcept :
    EditWindow{ patchTex1, resBinder, cmdList },
    m_patchTex1{ patchTex1 },
    m_combo{ make_unique<EditCombo>("Bind Keys", resBinder->GetTextureKeys(TextureSlice::One)) }
{}

void EditPatchTexture1::RenderComponent()
{
    auto resBinder = GetTextureResourceBinder();
    m_combo->Render([this, &resBinder](const string& curKey) {
        if (auto infoRef = resBinder->GetTextureSourceInfo(curKey); infoRef)
            m_patchTex1->ChangeBindKey(curKey, *infoRef);
        });
}

////////////////////////////////////////////////

EditPatchTexture3::~EditPatchTexture3() = default;
EditPatchTexture3::EditPatchTexture3(PatchTexture3* patchTex3, IRenderer*, TextureResourceBinder* resBinder, UICommandList* cmdList) noexcept :
    EditWindow{ patchTex3, resBinder, cmdList },
    m_patchTex3{ patchTex3 }
{
    DirectionType dirType = m_patchTex3->GetDirectionType();
    TextureSlice texSlice = (dirType == DirectionType::Horizontal) ? TextureSlice::ThreeH : TextureSlice::ThreeV;

    m_combo = make_unique<EditCombo>("Bind Keys", resBinder->GetTextureKeys(texSlice));
}

void EditPatchTexture3::RenderComponent()
{
    auto resBinder = GetTextureResourceBinder();
    m_combo->Render([this, &resBinder](const string& curKey) {
        if (auto infoRef = resBinder->GetTextureSourceInfo(curKey); infoRef)
            m_patchTex3->ChangeBindKey(curKey, *infoRef);
        });
}

////////////////////////////////////////////////

EditPatchTexture9::~EditPatchTexture9() = default;
EditPatchTexture9::EditPatchTexture9(PatchTexture9* patchTex9, IRenderer*, TextureResourceBinder* resBinder, UICommandList* cmdList) noexcept :
    EditWindow{ patchTex9, resBinder, cmdList },
    m_patchTex9{ patchTex9 },
    m_combo{ make_unique<EditCombo>("Bind Keys", resBinder->GetTextureKeys(TextureSlice::Nine)) }
{}

void EditPatchTexture9::RenderComponent()
{
    auto resBinder = GetTextureResourceBinder();
    m_combo->Render([this, &resBinder](const string& curKey) {
        if (auto infoRef = resBinder->GetTextureSourceInfo(curKey); infoRef)
            m_patchTex9->ChangeBindKey(curKey, *infoRef);
        });
}