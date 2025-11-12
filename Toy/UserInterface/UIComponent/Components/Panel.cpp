#include "pch.h"
#include "Panel.h"
#include "IRenderer.h"
#include "Shared/SerializerIO/SerializerIO.h"

Panel::~Panel() = default;
Panel::Panel()
{}

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

void Panel::ProcessIO(SerializerIO& serializer)
{
    UIComponent::ProcessIO(serializer);
}

//unique_ptr<UIComponent> CreateRootPanel(const string& name, const UILayout& layout, IRenderer* renderer)
//{
//    auto panel = CreateComponent<Panel>(layout);
//    if (!UIEx(panel).Rename(name)) return nullptr;
//    if (!UIEx(panel).RenameRegion("MainRegionEntry")) return nullptr;
//    renderer->AddRenderComponent(panel.get());
//
//    return panel;
//}
