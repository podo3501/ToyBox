#pragma once
#include "../UIComponent.h"

struct IRenderer;
class SerializerIO;
class UIModule;
class UIModul2;

class Panel : public UIComponent
{
public:
    Panel();
    ~Panel();
    Panel(const string& name, const UILayout& layout) noexcept;

    static ComponentID GetTypeStatic() { return ComponentID::Panel; }
    virtual ComponentID GetTypeID() const noexcept override { return GetTypeStatic(); }
    virtual void ProcessIO(SerializerIO& serializer) override;

    bool Setup(const UILayout& layout = {});
    void SetUIModule(UIModule* uiModule) noexcept { m_uiModule = uiModule; }
    void SetUIModul2(UIModul2* uiModul2) noexcept { m_uiModul2 = uiModul2; }
    inline UIModule* GetUIModule() const noexcept { return m_uiModule; }
    inline UIModul2* GetUIModul2() const noexcept { return m_uiModul2; }

protected:
    Panel(const Panel& other);
    virtual unique_ptr<UIComponent> CreateClone() const override;

private:
    UIModule* m_uiModule{ nullptr };
    UIModul2* m_uiModul2{ nullptr };
};

//unique_ptr<UIComponent> CreateRootPanel(const string& name, const UILayout& layout, IRenderer* renderer);
