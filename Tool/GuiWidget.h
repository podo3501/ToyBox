#pragma once
#include "../Include/IRenderer.h"

struct IRenderItem;

class GuiWidget : public IImguiItem
{
public:
    GuiWidget(IRenderer* renderer);
    ~GuiWidget();

    //IImguiItem
    virtual void Render(ImGuiIO* io) override;
    virtual void Update();

    bool Create(IRenderItem* renderItem);
    void SetVisible(bool visible) { m_visible = visible; }

private:
    IRenderer* m_renderer{ nullptr };
    IRenderItem* m_renderItem;
    ImTextureID m_textureID{};
    
    bool m_visible{ true };
};