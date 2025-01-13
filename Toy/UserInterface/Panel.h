#pragma once
#include "UIComponent.h"

class JsonOperation;

class Panel : public UIComponent
{
public:
    Panel();
    ~Panel();
    Panel(const string& name, const XMUINT2& size);
    //virtual unique_ptr<UIComponent> Clone() const override;
    //virtual const Rectangle& GetArea() const noexcept;

    //void AddComponent(unique_ptr<UIComponent>&& comp, const Vector2& pos);

    virtual void SerializeIO(JsonOperation& operation) override;

protected:
    Panel(const Panel& other);
    virtual unique_ptr<UIComponent> CreateClone() const override;

private:
    Rectangle m_area{};
};
