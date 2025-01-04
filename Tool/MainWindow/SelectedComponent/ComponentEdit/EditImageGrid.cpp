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

EditImageGrid::~EditImageGrid() = default;
EditImageGrid::EditImageGrid(IRenderer* renderer) noexcept :
    m_renderer{ renderer }
{}

void EditImageGrid::UpdateComponent(InputManager* inputManager)
{
    if (!m_textureWindow || !m_textureWindow->IsOpen()) return;

    m_textureWindow->Update(inputManager);
}

void EditImageGrid::RenderComponent(UIComponent* component, bool& posModify)
{
    if (m_textureWindow)
        m_textureWindow->Render();

    RenderComponentEdit(component, posModify);
}

//////////////////////////////////////////////////////

static unique_ptr<TextureWindow> CreateTextureWindow(
    IRenderer* renderer,
    const wstring& filename,
    UIComponent* component)
{
    unique_ptr<TextureWindow> textureWindow = make_unique<TextureWindow>(renderer,
        CreateSourceExtractor(renderer, filename, component));
    textureWindow->Create(filename);

    return textureWindow;
}

//////////////////////////////////////////////////////

EditImageGrid1::~EditImageGrid1() = default;
EditImageGrid1::EditImageGrid1(IRenderer* renderer) :
    EditImageGrid{ renderer }
{}

void EditImageGrid1::RenderComponentEdit(UIComponent* component, bool& posModify)
{
    ImageGrid1* imgGrid1 = ComponentCast<ImageGrid1*>(component);
    if (EditFilename("Filename", imgGrid1->Filename))
        m_renderer->LoadComponent(component);
    posModify |= EditRectangle("Source", imgGrid1->Source);

    ImGui::Spacing();

    if (ImGui::Button("Extract Textrue Area"))
    {
        if (!m_textureWindow)
            m_textureWindow = CreateTextureWindow(m_renderer, imgGrid1->Filename, component);
    }
}

////////////////////////////////////////////////

EditImageGrid3::~EditImageGrid3() = default;
EditImageGrid3::EditImageGrid3(IRenderer* renderer) :
    EditImageGrid{ renderer }
{}

void EditImageGrid3::RenderComponentEdit(UIComponent* component, bool& modify)
{
    ImageGrid3* imgGrid3 = ComponentCast<ImageGrid3*>(component);
    wstring filename{};
    assert(imgGrid3->GetFilename(filename));
    if (EditFilename("Filename", filename))
    {
        assert(imgGrid3->SetFilename(filename));
        m_renderer->LoadComponent(component);
    }
    
    SourceDivider srcDivider{};
    assert(imgGrid3->GetSourceAnd2Divider(srcDivider));
    if (EditSourceAndDivider("Source", "Deviders", srcDivider))
        assert(imgGrid3->SetSourceAnd2Divider(srcDivider));
}

////////////////////////////////////////////////

void EditImageGrid9::RenderComponent(UIComponent* component, bool& modify)
{
    ImageGrid9* imgGrid9 = ComponentCast<ImageGrid9*>(component);
}