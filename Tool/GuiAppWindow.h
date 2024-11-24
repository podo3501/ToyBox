#pragma once
#include "../Include/IRenderer.h"

struct IRenderScene;
class UIComponent;
class MainWindow;
class GuiWidget;

class GuiAppWindow : public IImguiComponent
{
public:
    ~GuiAppWindow();
    GuiAppWindow(IRenderer* renderer);

    bool Create(unique_ptr<IRenderScene> scene, const XMUINT2& size);

    //IImguiComponent
    virtual void Update() override;
    virtual void Render(ImGuiIO* io) override;

private:
    IRenderer* m_renderer;
    unique_ptr<IRenderScene> m_scene;
    UIComponent* m_component;
    ImTextureID m_textureID{};
    bool m_visible{ true };

    XMUINT2 m_size{};

    unique_ptr<MainWindow> m_mainWindow;
    unique_ptr<GuiWidget> m_guiWidget;
};
