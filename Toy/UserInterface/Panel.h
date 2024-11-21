#pragma once
#include "UIComponent.h"

class JsonOperation;

class Panel : public UIComponent
{
public:
    Panel();
    ~Panel();
    Panel(const Panel& other);
    Panel(const string& name, const Rectangle& rect);
    virtual unique_ptr<UIComponent> Clone() override;
    virtual const Rectangle& GetArea() const noexcept;

    void AddComponent(unique_ptr<UIComponent>&& comp, const Vector2& pos);

    virtual void SerializeIO(JsonOperation* operation) override;

private:
    Rectangle m_area{};
};
