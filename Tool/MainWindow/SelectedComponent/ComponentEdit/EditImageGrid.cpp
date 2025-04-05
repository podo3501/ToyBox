#include "pch.h"
#include "EditImageGrid.h"
#include "../Include/IRenderer.h"
#include "../Toy/UserInterface/UIComponent/Components/ImageGrid1.h"
#include "../Toy/UserInterface/UIComponent/Components/ImageGrid3.h"
#include "../Toy/UserInterface/UIComponent/Components/ImageGrid9.h"
#include "../Toy/UserInterface/Command/UICommandList/UICommandList.h"
#include "../Toy/UserInterface/TextureResourceBinder/TextureResourceBinder.h"
#include "../Toy/Utility.h"
#include "EditUtility/EditUtility.h"

EditImageGrid1::~EditImageGrid1() = default;
EditImageGrid1::EditImageGrid1(ImageGrid1* imgGrid1, IRenderer*, TextureResourceBinder* resBinder, UICommandList* cmdList) noexcept :
    EditWindow{ imgGrid1, resBinder, cmdList },
    m_imageGrid1{ imgGrid1 },
    m_combo{ make_unique<EditCombo>("Bind Keys", resBinder->GetTextureKeys(ImagePart::One)) }
{}

void EditImageGrid1::RenderComponent()
{
    auto resBinder = GetTextureResourceBinder();
    m_combo->Render([this, &resBinder](const string& curKey) {
        if (auto infoRef = resBinder->GetTextureSourceInfo(curKey); infoRef)
            m_imageGrid1->ChangeBindKey(curKey, *infoRef);
        });
}

////////////////////////////////////////////////

EditImageGrid3::~EditImageGrid3() = default;
EditImageGrid3::EditImageGrid3(ImageGrid3* imgGrid3, IRenderer*, TextureResourceBinder* resBinder, UICommandList* cmdList) noexcept :
    EditWindow{ imgGrid3, resBinder, cmdList },
    m_imageGrid3{ imgGrid3 }
{
    DirectionType dirType = m_imageGrid3->GetDirectionType();
    ImagePart imgPart = (dirType == DirectionType::Horizontal) ? ImagePart::ThreeH : ImagePart::ThreeV;

    m_combo = make_unique<EditCombo>("Bind Keys", resBinder->GetTextureKeys(imgPart));
}

void EditImageGrid3::RenderComponent()
{
    auto resBinder = GetTextureResourceBinder();
    m_combo->Render([this, &resBinder](const string& curKey) {
        if (auto infoRef = resBinder->GetTextureSourceInfo(curKey); infoRef)
            m_imageGrid3->ChangeBindKey(curKey, *infoRef);
        });
}

////////////////////////////////////////////////

EditImageGrid9::~EditImageGrid9() = default;
EditImageGrid9::EditImageGrid9(ImageGrid9* imgGrid9, IRenderer*, TextureResourceBinder* resBinder, UICommandList* cmdList) noexcept :
    EditWindow{ imgGrid9, resBinder, cmdList },
    m_imageGrid9{ imgGrid9 },
    m_combo{ make_unique<EditCombo>("Bind Keys", resBinder->GetTextureKeys(ImagePart::Nine)) }
{}

void EditImageGrid9::RenderComponent()
{
    auto resBinder = GetTextureResourceBinder();
    m_combo->Render([this, &resBinder](const string& curKey) {
        if (auto infoRef = resBinder->GetTextureSourceInfo(curKey); infoRef)
            m_imageGrid9->ChangeBindKey(curKey, *infoRef);
        });
}