#include "pch.h"
#include "Panel.h"
#include "BGImage.h"
#include "../Utility.h"

Rectangle EncompassingRectangle(const Rectangle& rect1, const Rectangle& rect2)
{
    int left = std::min(rect1.x, rect2.x);
    int top = std::min(rect1.y, rect2.y);
    int right = std::max(rect1.x + rect1.width, rect2.x + rect2.width);
    int bottom = std::max(rect1.y + rect1.height, rect2.y + rect2.height);

    return Rectangle(left, top, right - left, bottom - top);
}

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

void Panel::Update(const Vector2& resolution)
{
    for (const auto& i : m_renderItems)
    {
        BGImage* bgImg = static_cast<BGImage*>(i.second.get());
        bgImg->Update(i.first, resolution);
    }
}

void Panel::AddRenderItem(const Vector2& normalPos, unique_ptr<IRenderItem>&& renderItem)
{
    m_area = EncompassingRectangle(m_area, renderItem->GetArea());
    m_renderItems.emplace_back(make_pair(normalPos, move(renderItem)));
}

