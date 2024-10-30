#pragma once
#include "../Include/IRenderItem.h"

class Panel : public IRenderItem
{
public:
    Panel();
    ~Panel();

    virtual bool LoadResources(ILoadData* load) override;
    virtual void Render(IRender* render) override;
    virtual bool IsPicking(const Vector2& pos)  const noexcept override;
    virtual const Rectangle& GetArea() const noexcept;

    void Update(const Vector2& resolution);
    void AddRenderItem(const Vector2& normalPos, unique_ptr<IRenderItem>&& renderItem);

private:
    vector<pair<Vector2, unique_ptr<IRenderItem>>> m_renderItems;
    Rectangle m_area{};
};
