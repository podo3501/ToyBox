#pragma once
#include "IRenderer.h"

class UserInterfaceWindow;
class TextureResBinderWindow;

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

    UserInterfaceWindow* GetFocusUIWindow() const noexcept;
    TextureResBinderWindow* GetFocusTextureResBinderWindow() const noexcept;
    void SetUIWindow(unique_ptr<UserInterfaceWindow> uiWindow) noexcept;
    void SetTextureWindow(unique_ptr<TextureResBinderWindow> textureWindow) noexcept;
    IRenderer* GetRenderer() const noexcept { return m_renderer; }

private:
    IRenderer* m_renderer;
    unique_ptr<Tool::Config> m_config;
    unique_ptr<MenuBar> m_menuBar;
    ImTextureID m_textureID{};
    bool m_visible{ true };

    XMUINT2 m_size{};

    vector<unique_ptr<UserInterfaceWindow>> m_uiWindows;
    vector<unique_ptr<TextureResBinderWindow>> m_textureWindows;
};
