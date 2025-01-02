#include "pch.h"
#include "ComponentWindow.h"
#include "TextureWindow.h"
#include "../Include/IRenderer.h"
#include "../Toy/UserInterface/ImageGrid1.h"
#include "../Toy/UserInterface/ImageGrid9.h"
#include "../EditUtility.h"
#include "SourceExtractor.h"

ComponentWindow::~ComponentWindow()
{}
ComponentWindow::ComponentWindow() :
	m_component{ nullptr }
{}

void ComponentWindow::SetComponent(UIComponent* component)
{
	m_component = component;
	m_visible = (m_component != nullptr);
}

void ComponentWindow::Update(InputManager* inputManager)
{
    UpdateComponent(inputManager);
}

void ComponentWindow::Render(const ImGuiWindow* mainWindow)
{
    if (!m_visible) return;

    ImGui::Begin("Component Window - ", &m_visible, ImGuiWindowFlags_NoFocusOnAppearing);

    // Component 이름 수정
    static char buffer[128];
    snprintf(buffer, sizeof(buffer), "%s", m_component->GetName().c_str());

    if (ImGui::InputText("Name", buffer, IM_ARRAYSIZE(buffer)))
        m_component->SetName(buffer); // 이름 업데이트

    bool modify{ false };
    const auto& layout = m_component->GetLayout();
    modify |= EditSize(layout.GetSize());
    
    ImGui::Separator();
    ImGui::Spacing();

    RenderComponent(m_component, modify);
    if (modify)
        m_component->RefreshPosition();

    ImGui::End();
}

bool ComponentWindow::EditSize(const XMUINT2& size)
{
    XMUINT2 newSize = size;

    bool changed{ false };

    changed |= EditInteger("Width", newSize.x);
    changed |= EditInteger("Height", newSize.y);

    if (changed)
        m_component->ChangeSize(newSize);

    return changed;
}

////////////////////////////////////////////////

ComponentImageGrid1::~ComponentImageGrid1() = default;
ComponentImageGrid1::ComponentImageGrid1(IRenderer* renderer) :
    m_renderer{ renderer }
{}

void ComponentImageGrid1::UpdateComponent(InputManager* inputManager)
{
    if (m_textureWindow != nullptr && !m_textureWindow->IsOpen())
        m_textureWindow = nullptr;

    if (m_textureWindow)
        m_textureWindow->Update(inputManager);
}

void ComponentImageGrid1::RenderComponent(UIComponent* component, bool& posModify)
{
    if (m_textureWindow)
        m_textureWindow->Render();

    ImageGrid1* imgGrid1 = ComponentCast<ImageGrid1*>(component);
    if (EditFilename("Filename", imgGrid1->Filename))
        m_renderer->LoadComponent(component);
    posModify |= EditRectangle("Source", imgGrid1->Source);
    
    ImGui::Spacing();

    if (ImGui::Button("Extract Textrue Area"))
    {
        if (!m_textureWindow)
        {
            m_textureWindow = make_unique<TextureWindow>(m_renderer, 
                CreateSourceExtractor(m_renderer, imgGrid1->Filename, component));
            m_textureWindow->Create(imgGrid1->Filename);
        }
    }
}

////////////////////////////////////////////////

void ComponentImageGrid3::RenderComponent(UIComponent* component, bool& modify)
{
}

void ComponentImageGrid9::RenderComponent(UIComponent* component, bool& modify)
{
    ImageGrid9* imgGrid9 = ComponentCast<ImageGrid9*>(component);
}