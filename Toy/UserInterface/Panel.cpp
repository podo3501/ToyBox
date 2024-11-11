#include "pch.h"
#include "Panel.h"
#include "BGImage.h"
#include "../Utility.h"

Panel::Panel() {};
Panel::~Panel() = default;

const Rectangle& Panel::GetArea() const noexcept
{
    return m_area;
}

void Panel::AddComponent(unique_ptr<UIComponent>&& comp, const Vector2& pos)
{
    m_area = Rectangle::Union(m_area, comp->GetArea());
    UIComponent::AddComponent(move(comp), pos);
}

