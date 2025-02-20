#pragma once
#include "../UIComponent.h"

struct IRenderer;
class JsonOperation;

class Panel : public UIComponent
{
public:
    Panel();
    ~Panel();
    Panel(const string& name, const UILayout& layout) noexcept;

    static ComponentID GetTypeStatic() { return ComponentID::Panel; }
    virtual ComponentID GetTypeID() const noexcept override { return GetTypeStatic(); }
    virtual bool ImplementActiveUpdate() noexcept override;
    virtual void SerializeIO(JsonOperation& operation) override;

    inline void EnableChildMouseEvents(bool enable) noexcept { m_mouseEvents = enable; }


protected:
    Panel(const Panel& other);
    virtual unique_ptr<UIComponent> CreateClone() const override;

private:
    bool m_mouseEvents{ true };
};

unique_ptr<UIComponent> CreateRootPanel(const string& name, const UILayout& layout, IRenderer* renderer);
