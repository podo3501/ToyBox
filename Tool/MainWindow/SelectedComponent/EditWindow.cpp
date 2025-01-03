#include "pch.h"
#include "EditWindow.h"
#include "../Include/IRenderer.h"
#include "../Toy/UserInterface/UIComponent.h"
#include "../EditUtility.h"

EditWindow::~EditWindow()
{}
EditWindow::EditWindow() :
	m_component{ nullptr }
{}

void EditWindow::SetComponent(UIComponent* component)
{
	m_component = component;
	m_visible = (m_component != nullptr);
}

void EditWindow::Update(InputManager* inputManager)
{
    UpdateComponent(inputManager);
}

void EditWindow::Render(const ImGuiWindow* mainWindow)
{
    if (!m_visible) return;

    ImGui::Begin("Component Window - ", &m_visible, ImGuiWindowFlags_NoFocusOnAppearing);

    // Component 이름 수정
    EditText("Name", m_component->Name);
    
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

bool EditWindow::EditSize(const XMUINT2& size)
{
    XMUINT2 newSize = size;

    bool changed{ false };

    changed |= EditInteger("Width", newSize.x);
    changed |= EditInteger("Height", newSize.y);

    if (changed)
        m_component->ChangeSize(newSize);

    return changed;
}
