#include "pch.h"
#include "EditImageGrid.h"
#include "../Include/IRenderer.h"
#include "../TextureWindow.h"
#include "../Toy/UserInterface/UIComponent/Components/ImageGrid1.h"
#include "../Toy/UserInterface/UIComponent/Components/ImageGrid3.h"
#include "../Toy/UserInterface/UIComponent/Components/ImageGrid9.h"
#include "../Toy/UserInterface/Command/UICommandList/UICommandList.h"
#include "../Toy/UserInterface/TextureResourceBinder/TextureResourceBinder.h"
#include "../Toy/Utility.h"
#include "../../EditUtility.h"
#include "../SourceExtractor.h"

static optional<int> GetTextureIndexOf(const vector<string>& keys, const string& findKey) noexcept
{
    auto find = ranges::find_if(keys, [&findKey](const auto& key) { return key == findKey; });
    if (find == keys.end()) return nullopt;

    return static_cast<int>(distance(keys.begin(), find));
}

static void ChangeKeyFromComboBox(const vector<string>& texKeys, const string& currentKey, function<void(const string&)> OnChangeKey)
{
    vector<const char*> keys;
    for (const auto& str : texKeys)
        keys.push_back(str.c_str());

    static int keyComboIdx = 0;
    if (auto index = GetTextureIndexOf(texKeys, currentKey); index)
        keyComboIdx = *index;

    if (ImGui::Combo("Bind Keys", &keyComboIdx, keys.data(), static_cast<int>(keys.size())))
    {
        const string& curKey = keys[keyComboIdx];
        OnChangeKey(curKey);
    }
}

//////////////////////////////////////////////////////

EditImageGrid1::~EditImageGrid1() = default;
EditImageGrid1::EditImageGrid1(ImageGrid1* imgGrid1, IRenderer*, TextureResourceBinder* resBinder, UICommandList* cmdList) noexcept :
    EditWindow{ imgGrid1, resBinder, cmdList },
    m_imageGrid1{ imgGrid1 }
{}

void EditImageGrid1::RenderComponent()
{
    auto resBinder = GetTextureResourceBinder();
    const auto& texKeys = resBinder->GetTextureKeys(ImagePart::One);

    ChangeKeyFromComboBox(texKeys, m_imageGrid1->GetBindKey(), [this, &resBinder](const string& curKey) {
        if (auto infoRef = resBinder->GetTextureSourceInfo(curKey); infoRef)
            m_imageGrid1->ChangeBindKey(curKey, *infoRef);
        });
}

////////////////////////////////////////////////

EditImageGrid3::~EditImageGrid3() = default;
EditImageGrid3::EditImageGrid3(ImageGrid3* imgGrid3, IRenderer*, TextureResourceBinder* resBinder, UICommandList* cmdList) noexcept :
    EditWindow{ imgGrid3, resBinder, cmdList },
    m_imageGrid3{ imgGrid3 }
{}

void EditImageGrid3::RenderComponent()
{
    DirectionType dirType = m_imageGrid3->GetDirectionType();
    ImagePart imgPart = (dirType == DirectionType::Horizontal) ? ImagePart::ThreeH : ImagePart::ThreeV;

    auto srcBinder = GetTextureResourceBinder();
    const auto& texKeys = srcBinder->GetTextureKeys(imgPart);

    ChangeKeyFromComboBox(texKeys, m_imageGrid3->GetBindKey(), [this, &srcBinder](const string& curKey) {
        if (auto infoRef = srcBinder->GetTextureSourceInfo(curKey); infoRef)
            m_imageGrid3->ChangeBindKey(curKey, *infoRef);
        });
}

////////////////////////////////////////////////

EditImageGrid9::~EditImageGrid9() = default;
EditImageGrid9::EditImageGrid9(ImageGrid9* imgGrid9, IRenderer*, TextureResourceBinder* resBinder, UICommandList* cmdList) noexcept :
    EditWindow{ imgGrid9, resBinder, cmdList },
    m_imageGrid9{ imgGrid9 }
{}

void EditImageGrid9::RenderComponent()
{
    auto resBinder = GetTextureResourceBinder();
    const auto& texKeys = resBinder->GetTextureKeys(ImagePart::Nine);

    ChangeKeyFromComboBox(texKeys, m_imageGrid9->GetBindKey(), [this, &resBinder](const string& curKey) {
        if (auto infoRef = resBinder->GetTextureSourceInfo(curKey); infoRef)
            m_imageGrid9->ChangeBindKey(curKey, *infoRef);
        });
}