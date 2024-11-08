#include "pch.h"
#include "Panel.h"
#include "BGImage.h"
#include "../Utility.h"

Panel::Panel() {};
Panel::~Panel() = default;

bool Panel::LoadResources(ILoadData* load)
{
    for (const auto& i : m_renderItems)
        ReturnIfFalse(i.second->LoadResources(load));

    return true;
}

void Panel::Render(IRender* render)
{
    for (const auto& i : m_renderItems)
        i.second->Render(render);
}

bool Panel::IsPicking(const Vector2& pos)  const noexcept
{
    return ranges::any_of(m_renderItems, [&pos](const auto& i) {
        IRenderItem* item = i.second.get();
        return item->IsPicking(pos);
        });
}

const Rectangle& Panel::GetArea() const noexcept
{
    return m_area;
}

bool Panel::Update(const Vector2& position) noexcept
{
    for (const auto& i : m_renderItems)
    {
        BGImage* bgImg = static_cast<BGImage*>(i.second.get());
        ReturnIfFalse(bgImg->Update(i.first + position));
    }

    return true;
}

void Panel::AddRenderItem(const Vector2& normalPos, unique_ptr<IRenderItem>&& renderItem)
{
    m_area = Rectangle::Union(m_area, renderItem->GetArea());
    m_renderItems.emplace_back(make_pair(normalPos, move(renderItem)));
}

