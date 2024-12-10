#include "pch.h"
#include "ComponentWindow.h"
#include "../Toy/UserInterface/UIComponent.h"
#include "../Toy/UserInterface/UILayout.h"

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

    RenderRectangleInput(*m_component->GetLayout());

    ImGui::End();
}

// Rectangle 수정 UI 생성 함수
void ComponentWindow::RenderRectangleInput(UILayout& layout)
{
    const Rectangle& rect = layout.GetArea();
    Rectangle newRect = rect;

    bool changed{ false };

    changed |= RenderIntegerInput("X Area", newRect.x);
    changed |= RenderIntegerInput("Y Area", newRect.y);
    changed |= RenderIntegerInput("Width Area", newRect.width);
    changed |= RenderIntegerInput("Height Area", newRect.height);

    if (changed)
    {
        layout.Set(newRect);
        m_component->Update({}, nullptr);
    }
}

bool ComponentWindow::RenderIntegerInput(const char* label, long& value, int min, int max)
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

////////////////////////////////////////////////////////
//
//void ComponentPanel::SetComponent(UIComponent* component)
//{
//
//}
//
//void ComponentPanel::Render()
//{
//
//}
//
////////////////////////////////////////////////////////
//
//void ComponentBGImage::SetComponent(UIComponent* component)
//{
//
//}
//
//void ComponentBGImage::Render()
//{
//
//}