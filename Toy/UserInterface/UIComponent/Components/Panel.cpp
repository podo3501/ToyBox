#include "pch.h"
#include "Panel.h"
#include "IRenderer.h"
#include "../../JsonOperation/JsonOperation.h"

Panel::~Panel() = default;
Panel::Panel() : 
    m_uiModule{ nullptr }
{};

Panel::Panel(const Panel& other) :
    UIComponent{ other },
    m_uiModule{ nullptr }
{}

Panel::Panel(const string& name, const UILayout& layout) noexcept :
    UIComponent(name, layout),
    m_uiModule{ nullptr }
{}

unique_ptr<UIComponent> Panel::CreateClone() const
{
    return unique_ptr<Panel>(new Panel(*this));
}

bool Panel::Setup(const UILayout& layout)
{
    SetLayout(layout);
    return true;
}

void Panel::SerializeIO(JsonOperation& operation)
{
    UIComponent::SerializeIO(operation);
}

unique_ptr<UIComponent> CreateRootPanel(const string& name, const UILayout& layout, IRenderer* renderer)
{
    auto panel = CreateComponent<Panel>(layout);
    if (!UIEx(panel).Rename(name)) return nullptr;
    if (!UIEx(panel).RenameRegion("MainRegionEntry")) return nullptr;
    renderer->AddRenderComponent(panel.get());

    return panel;
}
