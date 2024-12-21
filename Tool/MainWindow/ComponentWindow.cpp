#include "pch.h"
#include "ComponentWindow.h"
#include "../Toy/UserInterface/UIComponent.h"
#include "../Toy/UserInterface/ImageGrid9.h"

ComponentWindow::~ComponentWindow() = default;
ComponentWindow::ComponentWindow() :
	m_component{ nullptr }
{}

void ComponentWindow::SetComponent(UIComponent* component)
{
	m_component = component;
	m_visible = (m_component != nullptr);
}

void ComponentWindow::Render()
{
    if (!m_visible) return;

    ImGui::Begin("Component Window - ", &m_visible, ImGuiWindowFlags_NoFocusOnAppearing);

    // Component 이름 수정
    static char buffer[128];
    snprintf(buffer, sizeof(buffer), "%s", m_component->GetName().c_str());

    if (ImGui::InputText("Name", buffer, IM_ARRAYSIZE(buffer)))
        m_component->SetName(buffer); // 이름 업데이트

    RenderRectangleInput(m_component->GetLayout());

    RenderComponent(m_component);

    ImGui::End();
}

// Rectangle 수정 UI 생성 함수
void ComponentWindow::RenderRectangleInput(const UILayout& layout)
{
    const XMUINT2& size = layout.GetSize();
    XMUINT2 newSize = size;

    bool changed{ false };

    changed |= RenderIntegerInput("Width", newSize.x);
    changed |= RenderIntegerInput("Height", newSize.y);

    if (changed)
        m_component->ChangeSize(newSize);
}

bool ComponentWindow::RenderIntegerInput(const char* label, uint32_t& value, int min, int max)
{
    int temp = value;
    if (ImGui::InputInt(label, &temp))
    {
        if (temp >= min && temp <= max)
        {
            value = temp;
            return true;
        }
    }
    return false;
}

////////////////////////////////////////////////

void ComponentImageGrid1::RenderComponent(UIComponent* component)
{
}

void ComponentImageGrid3::RenderComponent(UIComponent* component)
{
}

void ComponentImageGrid9::RenderComponent(UIComponent* component)
{
    ImageGrid9* imgGrid9 = ComponentCast<ImageGrid9*>(component);
}