#pragma once
#include "../Include/IRenderer.h"

struct IRenderItem;

class GuiWidget : public IImguiItem
{
public:
    GuiWidget();
    ~GuiWidget();

    //IImguiItem
    virtual void Render(ImGuiIO* io);

    void Update();
    bool Create(IRenderer* renderer, const XMUINT2& size, IRenderItem* renderItem);

private:
    ImTextureID m_textureID{ 0 };
    ImVec2 m_size{};

    IRenderItem* m_renderItem;
};