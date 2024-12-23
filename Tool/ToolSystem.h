#pragma once
#include "../Include/IRenderer.h"

class MainWindow;
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
class InputManager;

class ToolSystem : public IImguiComponent
{
public:
    ~ToolSystem();
    ToolSystem(IRenderer* renderer);

    void Update(const DX::StepTimer* timer, InputManager* inputManager);

    //IImguiComponent
    //virtual void Update() override;
    virtual void Render(ImGuiIO* io) override;

    MainWindow* GetFocusMainWindow() const noexcept;
    void SetMainWindow(unique_ptr<MainWindow> mainWindow) noexcept;
    IRenderer* GetRenderer() const noexcept { return m_renderer; }

private:
    IRenderer* m_renderer;
    unique_ptr<Tool::Config> m_config;
    unique_ptr<MenuBar> m_menuBar;
    unique_ptr<Tool::Dialog> m_dialog;    //modal 창을 띄워주는 클래스(다른것들도 들어가서 나중에 확장될 가능성이 있음)
    ImTextureID m_textureID{};
    bool m_visible{ true };

    XMUINT2 m_size{};

    vector<unique_ptr<MainWindow>> m_mainWindows;
    unique_ptr<GuiWidget> m_guiWidget;
};
