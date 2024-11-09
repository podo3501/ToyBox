#pragma once
#include "IRenderItem.h"

class Panel : public IRenderItem
{
public:
    Panel();
    ~Panel();

    virtual bool SetResources(const wstring& filename) override { filename; return true; }
    virtual bool LoadResources(ILoadData* load) override;
    virtual bool Update(const Vector2& position, const Mouse::ButtonStateTracker* tracker) noexcept override;
    virtual void Render(IRender* render) override;
    virtual bool IsPicking(const Vector2& pos)  const noexcept override;
    virtual const Rectangle& GetArea() const noexcept;
    virtual void SetSelected(const string& name, bool selected) noexcept {};
    virtual IRenderItem* GetSelected() const noexcept { return nullptr; }
    virtual unique_ptr<IRenderItem> Clone() { return nullptr; }
    virtual void SetPosition(const string& name, const Vector2&) noexcept override {};
    virtual bool ChangeArea(const Rectangle&) noexcept override { return true; }
    virtual const string& GetName() const noexcept { return m_name; }
    virtual IRenderItem* GetRenderItem(const string& name) const noexcept override { return nullptr; }

    void AddRenderItem(const Vector2& normalPos, unique_ptr<IRenderItem>&& renderItem);

private:
    string m_name{};
    vector<pair<Vector2, unique_ptr<IRenderItem>>> m_renderItems;
    Rectangle m_area{};
};
