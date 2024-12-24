#include "pch.h"
#include "ComponentWindow.h"
#include "../Toy/UserInterface/UIComponent.h"
#include "../Toy/UserInterface/ImageGrid1.h"
#include "../Toy/UserInterface/ImageGrid9.h"
#include "EditUtility.h"

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

    const auto& layout = m_component->GetLayout();
    EditSize(layout.GetSize());

    RenderComponent(m_component);

    ImGui::End();
}

void ComponentWindow::EditSize(const XMUINT2& size)
{
    XMUINT2 newSize = size;

    bool changed{ false };

    changed |= EditInteger("Width", newSize.x);
    changed |= EditInteger("Height", newSize.y);

    if (changed)
        m_component->ChangeSize(newSize);
}

////////////////////////////////////////////////

bool EditInt(const char* label, Property<int>& value, int min = 0, int max = 10000)
{
    int temp = value;

    if (!ImGui::InputInt(label, &temp)) return false;
    if (temp < min || temp > max) return false;

    value = temp;
    return true;
}

void ComponentImageGrid1::RenderComponent(UIComponent* component)
{
    ImageGrid1* imgGrid1 = ComponentCast<ImageGrid1*>(component);
    //EditInt("TempValue", imgGrid1->TempValue);
    //EditFilename(imgGrid1->GetFilename());
}

////////////////////////////////////////////////

void ComponentImageGrid3::RenderComponent(UIComponent* component)
{
}

void ComponentImageGrid9::RenderComponent(UIComponent* component)
{
    ImageGrid9* imgGrid9 = ComponentCast<ImageGrid9*>(component);
}