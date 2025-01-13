#include "pch.h"
#include "EditImageGrid.h"
#include "../Include/IRenderer.h"
#include "../TextureWindow.h"
#include "../Toy/UserInterface/ImageGrid1.h"
#include "../Toy/UserInterface/ImageGrid3.h"
#include "../Toy/UserInterface/ImageGrid9.h"
#include "../Toy/UserInterface/UIType.h"
#include "../Toy/Utility.h"
#include "../../EditUtility.h"
#include "../SourceExtractor.h"

static unique_ptr<TextureWindow> CreateTextureWindow( IRenderer* renderer, const wstring& filename, UIComponent* component)
{
    unique_ptr<TextureWindow> textureWindow = make_unique<TextureWindow>(renderer,
        CreateSourceExtractor(renderer, filename, component));
    textureWindow->Create(filename);

    return textureWindow;
}

//////////////////////////////////////////////////////

EditImageGrid::~EditImageGrid() = default;
EditImageGrid::EditImageGrid(UIComponent* component, IRenderer* renderer) noexcept :
    EditWindow{ component },
    m_renderer{ renderer }
{}

void EditImageGrid::UpdateComponent(const InputManager& inputManager)
{
    if (!m_textureWindow || !m_textureWindow->IsOpen()) return;

    m_textureWindow->Update(inputManager);
}

void EditImageGrid::RenderComponent(bool& posModify)
{
    if (m_textureWindow)
        m_textureWindow->Render();

    RenderComponentEdit(posModify);
}

void EditImageGrid::RenderExtractTextureButton(const wstring& filename, UIComponent* component)
{
    if (ImGui::Button("Extract Textrue Area"))
    {
        if (!m_textureWindow)
            m_textureWindow = CreateTextureWindow(m_renderer, filename, component);
        else
            m_textureWindow->Open();
    }
}

//////////////////////////////////////////////////////

EditImageGrid1::~EditImageGrid1() = default;
EditImageGrid1::EditImageGrid1(ImageGrid1* imgGrid1, IRenderer* renderer) noexcept :
    EditImageGrid{ imgGrid1, renderer },
    m_imageGrid1{ imgGrid1 }
{}

void EditImageGrid1::RenderComponentEdit(bool& posModify)
{
    if (EditFilename("Filename", m_imageGrid1->Filename))
        GetRenderer()->LoadComponent(m_imageGrid1);
    posModify |= EditRectangle("Source", m_imageGrid1->Source);

    ImGui::Spacing();
    
    RenderExtractTextureButton(m_imageGrid1->Filename, m_imageGrid1);
}

////////////////////////////////////////////////

EditImageGrid3::~EditImageGrid3() = default;
EditImageGrid3::EditImageGrid3(ImageGrid3* imgGrid3, IRenderer* renderer) noexcept :
    EditImageGrid{ imgGrid3, renderer },
    m_imageGrid3{ imgGrid3 }
{}

void EditImageGrid3::RenderComponentEdit(bool& modify)
{
    wstring filename{};
    assert(m_imageGrid3->GetFilename(filename));
    if (EditFilename("Filename", filename))
    {
        assert(m_imageGrid3->SetFilename(filename));
        GetRenderer()->LoadComponent(m_imageGrid3);
    }
    
    SourceDivider srcDivider{};
    assert(m_imageGrid3->GetSourceAnd2Divider(srcDivider));
    if (EditSourceAndDivider("Source", "Deviders", srcDivider))
        assert(m_imageGrid3->SetSourceAnd2Divider(srcDivider));

    ImGui::Spacing();

    RenderExtractTextureButton(filename, m_imageGrid3);
}

////////////////////////////////////////////////

EditImageGrid9::~EditImageGrid9() = default;
EditImageGrid9::EditImageGrid9(ImageGrid9* imgGrid9, IRenderer* renderer) noexcept :
    EditImageGrid{ imgGrid9, renderer },
    m_imageGrid9{ imgGrid9 }
{}

void EditImageGrid9::RenderComponentEdit(bool& modify)
{
    wstring filename{};
    assert(m_imageGrid9->GetFilename(filename));
    if (EditFilename("Filename", filename))
    {
        assert(m_imageGrid9->SetFilename(filename));
        GetRenderer()->LoadComponent(m_imageGrid9);
    }

    SourceDivider srcDivider{};
    assert(m_imageGrid9->GetSourceAnd4Divider(srcDivider));
    if (EditSourceAndDivider("Source", "Deviders", srcDivider))
        assert(m_imageGrid9->SetSourceAnd4Divider(srcDivider));

    ImGui::Spacing();

    RenderExtractTextureButton(filename, m_imageGrid9);
}