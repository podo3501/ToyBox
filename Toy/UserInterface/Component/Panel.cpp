#include "pch.h"
#include "Panel.h"
#include "../../Utility.h"
#include "../JsonOperation.h"
#include "../Include/IRenderer.h"

Panel::Panel() {};
Panel::~Panel() = default;

Panel::Panel(const Panel& other) :
    UIComponent{ other }
{
    m_area = other.m_area;
}

Panel::Panel(const string& name, const UILayout& layout) noexcept :
    UIComponent(name, layout)
{}

unique_ptr<UIComponent> Panel::CreateClone() const
{
    return unique_ptr<Panel>(new Panel(*this));
}

//unique_ptr<UIComponent> Panel::Clone() const
//{
//    auto clone = std::unique_ptr<Panel>(new Panel(*this));  //make_unique를 쓰면 protected 생성자에 접근할 수 없다.
//    clone->SetName(clone->GetName() + "_clone");
//    return clone;
//}

//const Rectangle& Panel::GetArea() const noexcept
//{
//    return m_area;
//}
//
//void Panel::AddComponent(unique_ptr<UIComponent>&& comp, const Vector2& pos)
//{
//    m_area = Rectangle::Union(m_area, comp->GetArea());
//    UIComponent::AddComponent(move(comp), pos);
//}

void Panel::SerializeIO(JsonOperation& operation)
{
    //operation->Process("Area", m_area);
    UIComponent::SerializeIO(operation);
}

unique_ptr<UIComponent> CreateRootPanel(const string& name, const UILayout& layout, IRenderer* renderer)
{
    unique_ptr<UIComponent> panel = make_unique<Panel>(name, layout);
    panel->RenameRegion("MainRegionEntry");
    renderer->AddRenderComponent(panel.get());

    return panel;
}
