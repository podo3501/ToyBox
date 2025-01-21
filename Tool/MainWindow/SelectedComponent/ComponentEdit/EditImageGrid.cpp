#include "pch.h"
#include "EditImageGrid.h"
#include "../Include/IRenderer.h"
#include "../TextureWindow.h"
#include "../Toy/UserInterface/Component/ImageGrid1.h"
#include "../Toy/UserInterface/Component/ImageGrid3.h"
#include "../Toy/UserInterface/Component/ImageGrid9.h"
#include "../Toy/UserInterface/Command/CommandList.h"
#include "../Toy/Utility.h"
#include "../../EditUtility.h"
#include "../SourceExtractor.h"

static unique_ptr<TextureWindow> CreateTextureWindow(IRenderer* renderer, 
    const wstring& filename, UIComponent* component, CommandList* cmdList)
{
    unique_ptr<TextureWindow> textureWindow = make_unique<TextureWindow>(renderer,
        CreateSourceExtractor(renderer, filename, component, cmdList));
    textureWindow->Create(filename);

    return textureWindow;
}

//////////////////////////////////////////////////////

EditImageGrid::~EditImageGrid() = default;
EditImageGrid::EditImageGrid(UIComponent* component, IRenderer* renderer, CommandList* cmdList) noexcept :
    EditWindow{ component, cmdList },
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
            m_textureWindow = CreateTextureWindow(m_renderer, filename, component, GetCommandList());
        else
            m_textureWindow->Open();
    }
}

//////////////////////////////////////////////////////

EditImageGrid1::~EditImageGrid1() = default;
EditImageGrid1::EditImageGrid1(ImageGrid1* imgGrid1, IRenderer* renderer, CommandList* cmdList) noexcept :
    EditImageGrid{ imgGrid1, renderer, cmdList },
    m_imageGrid1{ imgGrid1 }
{}

void EditImageGrid1::RenderComponentEdit(bool& posModify)
{
    if (EditFilename("Filename", m_imageGrid1->Filename))
        GetRenderer()->LoadComponent(m_imageGrid1);

    Rectangle source = m_imageGrid1->GetSource();
    if (EditRectangle("Source", source))
    {
        GetCommandList()->Source(m_imageGrid1, source);
        posModify |= true;
    }

    ImGui::Spacing();
    
    RenderExtractTextureButton(m_imageGrid1->Filename, m_imageGrid1);
}

////////////////////////////////////////////////

EditImageGrid3::~EditImageGrid3() = default;
EditImageGrid3::EditImageGrid3(ImageGrid3* imgGrid3, IRenderer* renderer, CommandList* cmdList) noexcept :
    EditImageGrid{ imgGrid3, renderer, cmdList },
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
EditImageGrid9::EditImageGrid9(ImageGrid9* imgGrid9, IRenderer* renderer, CommandList* cmdList) noexcept :
    EditImageGrid{ imgGrid9, renderer, cmdList },
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