#include "pch.h"
#include "EditImageGrid.h"
#include "../Include/IRenderer.h"
#include "../TextureWindow.h"
#include "../Toy/UserInterface/UIComponent/Components/ImageGrid1.h"
#include "../Toy/UserInterface/UIComponent/Components/ImageGrid3.h"
#include "../Toy/UserInterface/UIComponent/Components/ImageGrid9.h"
#include "../Toy/UserInterface/Command/UICommandList.h"
#include "../Toy/UserInterface/TextureSourceBinder/TextureSourceBinder.h"
#include "../Toy/Utility.h"
#include "../../EditUtility.h"
#include "../SourceExtractor.h"

//static unique_ptr<TextureWindow> CreateTextureWindow(IRenderer* renderer, 
//    const wstring& filename, UIComponent* component, UICommandList* cmdList)
//{
//    unique_ptr<TextureWindow> textureWindow = make_unique<TextureWindow>(renderer,
//        CreateSourceExtractor(renderer, filename, component, cmdList));
//    textureWindow->Create(filename);
//
//    return textureWindow;
//}
//
////////////////////////////////////////////////////////
//
//EditImageGrid::~EditImageGrid() = default;
//EditImageGrid::EditImageGrid(UIComponent* component, IRenderer* renderer, TextureSourceBinder* sourceBinder, UICommandList* cmdList) noexcept :
//    EditWindow{ component, sourceBinder, cmdList },
//    m_renderer{ renderer }
//{}
//
//void EditImageGrid::UpdateComponent()
//{
//    if (!m_textureWindow && !m_filename.empty())
//    {
//        m_textureWindow = CreateTextureWindow(m_renderer, m_filename, GetComponent(), GetUICommandList());
//        m_filename.clear();
//    }
//
//    if (!m_textureWindow || !m_textureWindow->IsOpen()) return;
//
//    m_textureWindow->Update();
//}
//
//void EditImageGrid::RenderComponent()
//{
//    if (m_textureWindow)
//        m_textureWindow->Render();
//
//    RenderComponentEdit();
//}
//
//void EditImageGrid::RenderExtractTextureButton(const wstring& filename, UIComponent* component)
//{
//    if (ImGui::Button("Extract Texture Area"))
//    {
//        if (!m_textureWindow)
//            m_filename = filename;
//        else
//            m_textureWindow->Open();
//    }
//}

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
EditImageGrid1::EditImageGrid1(ImageGrid1* imgGrid1, IRenderer*, TextureSourceBinder* sourceBinder, UICommandList* cmdList) noexcept :
    EditWindow{ imgGrid1, sourceBinder, cmdList },
    m_imageGrid1{ imgGrid1 }
{}

void EditImageGrid1::RenderComponent()
{
    auto srcBinder = GetTextureSourceBinder();
    const auto& texKeys = srcBinder->GetTextureKeys(ImagePart::One);

    ChangeKeyFromComboBox(texKeys, m_imageGrid1->GetBindKey(), [this, &srcBinder](const string& curKey) {
        if (auto infoRef = srcBinder->GetTextureSourceInfo(curKey); infoRef)
            m_imageGrid1->ChangeBindKey(curKey, *infoRef);
        });
}

////////////////////////////////////////////////

EditImageGrid3::~EditImageGrid3() = default;
EditImageGrid3::EditImageGrid3(ImageGrid3* imgGrid3, IRenderer*, TextureSourceBinder* sourceBinder, UICommandList* cmdList) noexcept :
    EditWindow{ imgGrid3, sourceBinder, cmdList },
    m_imageGrid3{ imgGrid3 }
{}

void EditImageGrid3::RenderComponent()
{
    DirectionType dirType = m_imageGrid3->GetDirectionType();
    ImagePart imgPart = (dirType == DirectionType::Horizontal) ? ImagePart::ThreeH : ImagePart::ThreeV;

    auto srcBinder = GetTextureSourceBinder();
    const auto& texKeys = srcBinder->GetTextureKeys(imgPart);

    ChangeKeyFromComboBox(texKeys, m_imageGrid3->GetBindKey(), [this, &srcBinder](const string& curKey) {
        if (auto infoRef = srcBinder->GetTextureSourceInfo(curKey); infoRef)
            m_imageGrid3->ChangeBindKey(curKey, *infoRef);
        });
}

////////////////////////////////////////////////

EditImageGrid9::~EditImageGrid9() = default;
EditImageGrid9::EditImageGrid9(ImageGrid9* imgGrid9, IRenderer*, TextureSourceBinder* sourceBinder, UICommandList* cmdList) noexcept :
    EditWindow{ imgGrid9, sourceBinder, cmdList },
    m_imageGrid9{ imgGrid9 }
{}

void EditImageGrid9::RenderComponent()
{
    auto srcBinder = GetTextureSourceBinder();
    const auto& texKeys = srcBinder->GetTextureKeys(ImagePart::Nine);

    ChangeKeyFromComboBox(texKeys, m_imageGrid9->GetBindKey(), [this, &srcBinder](const string& curKey) {
        if (auto infoRef = srcBinder->GetTextureSourceInfo(curKey); infoRef)
            m_imageGrid9->ChangeBindKey(curKey, *infoRef);
        });
}