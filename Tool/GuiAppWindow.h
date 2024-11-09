#pragma once
#include "../Include/IRenderer.h"

struct IRenderScene;
struct IRenderItem;
class GuiWidget;

class GuiAppWindow : public IImguiItem
{
public:
    ~GuiAppWindow();
    GuiAppWindow(IRenderer* renderer);

    bool Create(unique_ptr<IRenderScene> scene, const XMUINT2& size);

    //IImguiItem
    virtual void Update() override;
    virtual void Render(ImGuiIO* io) override;

private:
    IRenderer* m_renderer;
    unique_ptr<IRenderScene> m_scene;
    IRenderItem* m_renderItem;
    ImTextureID m_textureID{};
    bool m_visible{ true };

    XMUINT2 m_size{};

    unique_ptr<GuiWidget> m_guiWidget;
};
