#pragma once
#include "../UIComponent.h"

struct IRenderer;
class JsonOperation;
class UIModule;

class Panel : public UIComponent
{
public:
    Panel();
    ~Panel();
    Panel(const string& name, const UILayout& layout) noexcept;

    static ComponentID GetTypeStatic() { return ComponentID::Panel; }
    virtual ComponentID GetTypeID() const noexcept override { return GetTypeStatic(); }
    virtual void SerializeIO(JsonOperation& operation) override;

    bool Setup(const UILayout& layout);
    void SetUIModule(UIModule* uiModule) noexcept { m_uiModule = uiModule; }
    inline UIModule* GetUIModule() const noexcept { return m_uiModule; }

protected:
    Panel(const Panel& other);
    virtual unique_ptr<UIComponent> CreateClone() const override;

private:
    UIModule* m_uiModule;
};

unique_ptr<UIComponent> CreateRootPanel(const string& name, const UILayout& layout, IRenderer* renderer);
