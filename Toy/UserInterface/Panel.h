#pragma once
#include "IRenderItem.h"

class Panel : public IRenderItem
{
public:
    Panel();
    ~Panel();

    virtual bool SetResources(const wstring& filename) override { filename; return true; }
    virtual const Rectangle& GetArea() const noexcept;
    virtual unique_ptr<IRenderItem> Clone() { return nullptr; }

    void AddComponent(unique_ptr<IRenderItem>&& item, const Vector2& pos);

private:
    Rectangle m_area{};
};
