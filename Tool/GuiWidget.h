#pragma once
#include "../Include/IRenderer.h"

class GuiWidget : public IImguiItem
{
public:
    GuiWidget();
    ~GuiWidget();

    //IImguiItem
    virtual void Render(ImGuiIO* io);

    bool Create(IRenderer* renderer, const XMUINT2& size, IRenderItem* renderItem);

private:
    ImTextureID m_textureID{ 0 };
    ImVec2 m_size{};
};