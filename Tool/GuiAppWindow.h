#pragma once
#include "../Include/IRenderer.h"

class GuiWidget;

class GuiAppWindow : public IImguiItem
{
public:
    ~GuiAppWindow();
    GuiAppWindow(IRenderer* renderer);

    bool Create(IRenderItem* renderItem, const XMUINT2& size);

    //IImguiItem
    virtual void Update() override;
    virtual void Render(ImGuiIO* io) override;

private:
    IRenderer* m_renderer;
    IRenderItem* m_renderItem;
    ImTextureID m_textureID{};
    bool m_visible{ true };

    XMUINT2 m_size{};

    unique_ptr<GuiWidget> m_guiWidget;
};
