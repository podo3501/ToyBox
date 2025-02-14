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

void EditImageGrid::UpdateComponent()
{
    if (!m_textureWindow && !m_filename.empty())
    {
        m_textureWindow = CreateTextureWindow(m_renderer, m_filename, GetComponent(), GetCommandList());
        m_filename.clear();
    }

    if (!m_textureWindow || !m_textureWindow->IsOpen()) return;

    m_textureWindow->Update();
}

void EditImageGrid::RenderComponent()
{
    if (m_textureWindow)
        m_textureWindow->Render();

    RenderComponentEdit();
}

void EditImageGrid::RenderExtractTextureButton(const wstring& filename, UIComponent* component)
{
    if (ImGui::Button("Extract Textrue Area"))
    {
        if (!m_textureWindow)
            m_filename = filename;
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

void EditImageGrid1::RenderComponentEdit()
{
    wstring filename{ *m_imageGrid1->GetFilename() };
    if (EditFilename("Filename", filename))
        GetCommandList()->SetFilename(m_imageGrid1, GetRenderer(), filename);

    Rectangle source = m_imageGrid1->GetSource();
    if (EditRectangle("Source", source))
        GetCommandList()->SetSource(m_imageGrid1, source);

    ImGui::Spacing();
    
    RenderExtractTextureButton(*m_imageGrid1->GetFilename(), m_imageGrid1);
}

////////////////////////////////////////////////

EditImageGrid3::~EditImageGrid3() = default;
EditImageGrid3::EditImageGrid3(ImageGrid3* imgGrid3, IRenderer* renderer, CommandList* cmdList) noexcept :
    EditImageGrid{ imgGrid3, renderer, cmdList },
    m_imageGrid3{ imgGrid3 }
{}

void EditImageGrid3::RenderComponentEdit()
{
    wstring filename{ *m_imageGrid3->GetFilename() };
    if (EditFilename("Filename", filename))
        GetCommandList()->SetFilename(m_imageGrid3, GetRenderer(), filename);
    
    SourceDivider srcDivider{ *m_imageGrid3->GetSourceAnd2Divider() };
    if (EditSourceAndDivider("Source", "Deviders", srcDivider))
        GetCommandList()->SetSourceAndDivider(m_imageGrid3, srcDivider);

    ImGui::Spacing();

    RenderExtractTextureButton(filename, m_imageGrid3);
}

////////////////////////////////////////////////

EditImageGrid9::~EditImageGrid9() = default;
EditImageGrid9::EditImageGrid9(ImageGrid9* imgGrid9, IRenderer* renderer, CommandList* cmdList) noexcept :
    EditImageGrid{ imgGrid9, renderer, cmdList },
    m_imageGrid9{ imgGrid9 }
{}

void EditImageGrid9::RenderComponentEdit()
{
    wstring filename{ *m_imageGrid9->GetFilename() };
    if (EditFilename("Filename", filename))
        GetCommandList()->SetFilename(m_imageGrid9, GetRenderer(), filename);
    
    SourceDivider srcDivider{ *m_imageGrid9->GetSourceAnd4Divider() };
    if (EditSourceAndDivider("Source", "Deviders", srcDivider))
        GetCommandList()->SetSourceAndDivider(m_imageGrid9, srcDivider);

    ImGui::Spacing();

    RenderExtractTextureButton(filename, m_imageGrid9);
}