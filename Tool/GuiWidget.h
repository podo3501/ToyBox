#pragma once
#include "../Include/IRenderer.h"

struct IRenderItem;
struct IRenderScene;

class GuiWidget : public IImguiItem
{
public:
    GuiWidget(IRenderer* renderer);
    ~GuiWidget();

    //IImguiItem
    virtual void Render(ImGuiIO* io) override;
    virtual void Update();

    bool Create(unique_ptr<IRenderItem> renderItem);
    void SetVisible(bool visible) { m_visible = visible; }

private:
    IRenderer* m_renderer{ nullptr };
    unique_ptr<IRenderScene> m_widgetScene;
    IRenderItem* m_renderItem;
    ImTextureID m_textureID{};
    
    bool m_visible{ true };
};