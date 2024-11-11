#pragma once
#include "../Include/IRenderer.h"

struct IRenderScene;
class UIComponent;

class GuiWidget : public IImguiComponent
{
public:
    GuiWidget(IRenderer* renderer);
    ~GuiWidget();

    //IImguiComponent
    virtual void Render(ImGuiIO* io) override;
    virtual void Update();

    bool Create(unique_ptr<UIComponent> comp);
    void SetVisible(bool visible) { m_visible = visible; }

private:
    IRenderer* m_renderer{ nullptr };
    unique_ptr<IRenderScene> m_widgetScene;
    UIComponent* m_component;
    ImTextureID m_textureID{};
    
    bool m_visible{ true };
};