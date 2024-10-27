#pragma once
#include "../Include/IRenderer.h"

struct IRenderItem;

class GuiWidget : public IImguiItem
{
public:
    GuiWidget(IRenderer* renderer);
    ~GuiWidget();

    //IImguiItem
    virtual void Render(ImGuiIO* io);

    void Update();
    bool Create(IRenderer* renderer, IRenderItem* renderItem);

private:
    IRenderer* m_renderer{ nullptr };
    ImTextureID m_textureID{ 0 };

    IRenderItem* m_renderItem;
};