#include "pch.h"
#include "Panel.h"
#include "../../Utility.h"
#include "../JsonOperation.h"
#include "../Include/IRenderer.h"
#include "../../InputManager.h"

Panel::Panel() {};
Panel::~Panel() = default;

Panel::Panel(const Panel& other) :
    UIComponent{ other }
{}

Panel::Panel(const string& name, const UILayout& layout) noexcept :
    UIComponent(name, layout)
{}

unique_ptr<UIComponent> Panel::CreateClone() const
{
    return unique_ptr<Panel>(new Panel(*this));
}

bool Panel::ImplementActiveUpdate() noexcept
{
    CheckMouseInArea();
    CheckEnterArea();
    SetStateFlag(StateFlag::ActiveUpdate, m_mouseInArea && m_mouseEvents);
    
    return true;
}

void Panel::CheckMouseInArea() noexcept
{
    auto rect = GetRectangle();
    const XMINT2& mousePos = InputManager::GetMouse().GetPosition();

    m_mouseInArea = rect.Contains(mousePos.x, mousePos.y);
}

void Panel::CheckEnterArea() noexcept
{
    m_entered = !m_lastMouseInArea && m_mouseInArea;
    m_lastMouseInArea = m_mouseInArea;
}

void Panel::SerializeIO(JsonOperation& operation)
{
    UIComponent::SerializeIO(operation);
}

unique_ptr<UIComponent> CreateRootPanel(const string& name, const UILayout& layout, IRenderer* renderer)
{
    unique_ptr<UIComponent> panel = make_unique<Panel>(name, layout);
    panel->RenameRegion("MainRegionEntry");
    renderer->AddRenderComponent(panel.get());

    return panel;
}
