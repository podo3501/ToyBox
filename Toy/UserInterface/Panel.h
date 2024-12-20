#pragma once
#include "UIComponent.h"

class JsonOperation;

class Panel : public UIComponent
{
protected:
    Panel(const Panel& other);
    virtual unique_ptr<UIComponent> CreateClone() const override;

public:
    Panel();
    ~Panel();
    Panel(const string& name, const Rectangle& rect);
    //virtual unique_ptr<UIComponent> Clone() const override;
    //virtual const Rectangle& GetArea() const noexcept;

    //void AddComponent(unique_ptr<UIComponent>&& comp, const Vector2& pos);

    virtual void SerializeIO(JsonOperation& operation) override;

private:
    Rectangle m_area{};
};
