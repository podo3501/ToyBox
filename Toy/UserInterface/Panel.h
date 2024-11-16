#pragma once
#include "UIComponent.h"

class Panel : public UIComponent
{
public:
    Panel();
    ~Panel();

    //virtual bool SetResources(const wstring& filename) override { filename; return true; }
    virtual const Rectangle& GetArea() const noexcept;
    virtual unique_ptr<UIComponent> Clone() { return nullptr; }

    void AddComponent(unique_ptr<UIComponent>&& comp, const Vector2& pos);

private:
    Rectangle m_area{};
};
