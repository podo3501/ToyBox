#pragma once
#include "../Include/IRenderer.h"

class MainWindow;
class TextureResBinderWindow;
class GuiWidget;

namespace DX
{
    class StepTimer;
}

namespace Tool
{
    class Dialog;
    class Config;
}

class MenuBar;

class ToolSystem : public IImguiComponent
{
public:
    ~ToolSystem();
    ToolSystem(IRenderer* renderer);

    void Update(const DX::StepTimer& timer);

    //IImguiComponent
    //virtual void Update() override;
    virtual void Render(ImGuiIO* io) override;

    MainWindow* GetFocusMainWindow() const noexcept;
    TextureResBinderWindow* GetFocusTextureResBinderWindow() const noexcept;
    void SetMainWindow(unique_ptr<MainWindow> mainWindow) noexcept;
    void SetTextureWindow(unique_ptr<TextureResBinderWindow> textureWindow) noexcept;
    IRenderer* GetRenderer() const noexcept { return m_renderer; }

private:
    IRenderer* m_renderer;
    unique_ptr<Tool::Config> m_config;
    unique_ptr<MenuBar> m_menuBar;
    ImTextureID m_textureID{};
    bool m_visible{ true };

    XMUINT2 m_size{};

    vector<unique_ptr<MainWindow>> m_mainWindows;
    vector<unique_ptr<TextureResBinderWindow>> m_textureWindows;
    unique_ptr<GuiWidget> m_guiWidget;
};
