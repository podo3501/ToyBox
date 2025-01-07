#include "pch.h"
#include "EditWindow.h"
#include "../Include/IRenderer.h"
#include "../Toy/UserInterface/UIComponent.h"
#include "../Toy/UserInterface/Panel.h"
#include "../EditUtility.h"

EditWindow::~EditWindow()
{
}

EditWindow::EditWindow(UIComponent* component) noexcept:
	m_component{ component }
{}

void EditWindow::Update(InputManager* inputManager)
{
    UpdateComponent(inputManager);
}

void EditWindow::RenderCommon(bool& modify)
{
    // Component 이름 수정
    EditText("Name", m_component->Name);

    XMINT2 relativePosition{};
    bool isExist = m_component->GetRelativePosition(relativePosition);
    if(isExist)
    {
        modify |= EditInteger("X", relativePosition.x);
        modify |= EditInteger("Y", relativePosition.y);

        if (modify)
            m_component->SetRelativePosition(relativePosition);
    }

    const auto& layout = m_component->GetLayout();
    modify |= EditSize(layout.GetSize());

    ImGui::Separator();
    ImGui::Spacing();
}

void EditWindow::Render(const ImGuiWindow* mainWindow)
{
    if (!m_visible) return;

    ImGui::Begin("Component Window - ", &m_visible, ImGuiWindowFlags_NoFocusOnAppearing);
    
    bool modify{ false };

    RenderCommon(modify);
    RenderComponent(modify);

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

//////////////////////////////////////////////////////////

EditPanel::~EditPanel() = default;
EditPanel::EditPanel(Panel* panel) noexcept :
    EditWindow{ panel }
{
}