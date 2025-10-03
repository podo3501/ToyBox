#pragma once
#include "../InnerWindow.h"

namespace DX
{
    class StepTimer;
}

class Panel;
class MouseTracker;
class RenderTexture;
class TextureResourceBinder;
class ComponentController;
class UIModule;

class UserInterfaceWindow : public InnerWindow
{
public:
    UserInterfaceWindow(IRenderer* renderer);
    ~UserInterfaceWindow();

    virtual void Render(ImGuiIO* io) override;

    bool CreateScene(const XMUINT2& size);
    bool CreateScene(const wstring& filename);
    bool SaveScene(const wstring& filename);
    wstring GetSaveFilename() const noexcept;
    void Update(const DX::StepTimer& timer);
    void ChangeWindowSize(const ImVec2& size);

    inline bool IsOpen() const noexcept { return m_isOpen; }

private:
    bool SetupProperty(unique_ptr<UIModule> uiModule);
    void ToggleToolMode() noexcept;
    void CheckActiveUpdate() noexcept;
    void CheckChangeWindow(const ImGuiWindow* window);
    UIModule* GetUIModule() const noexcept;
    inline ImVec2 GetPanelSize() const noexcept;

    void SetupWindowAppearing() noexcept;
    void SetupImGuiWindow();
    void RenderContent();
    void ShowStatusBar() const;
    void HandleMouseEvents();

    IRenderer* m_renderer;
    ImGuiWindow* m_window{ nullptr };
    unique_ptr<ComponentController> m_controller;
    unique_ptr<RenderTexture> m_mainRenderTexture;

    bool m_isOpen{ false };
    bool m_isTool{ false };
    float m_fps{ 0.f };

    static int m_uiWindowIndex;
};
