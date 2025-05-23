#include "pch.h"
#include "Panel.h"
#include "../Include/IRenderer.h"
#include "Utility.h"
#include "../../JsonOperation/JsonOperation.h"


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
    if (!panel->Rename(name)) return nullptr;
    if (!panel->RenameRegion("MainRegionEntry")) return nullptr;
    renderer->AddRenderComponent(panel.get());

    return panel;
}
