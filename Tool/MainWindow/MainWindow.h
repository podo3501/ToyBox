#pragma once
#include "../Include/IRenderer.h"

namespace DX
{
    class StepTimer;
}

class UIComponent;
class MouseTracker;
class ComponentController;

class MainWindow : public IImguiComponent
{
public:
    MainWindow(IRenderer* renderer);
    ~MainWindow();

    virtual void Render(ImGuiIO* io) override;

    bool CreateScene(const XMUINT2& size);
    bool CreateScene(const wstring& filename);
    bool SaveScene(const wstring& filename);
    wstring GetSaveFilename() const noexcept;
    void Update(const DX::StepTimer* timer);
    void ChangeWindowSize(const ImVec2& size);

    inline bool IsOpen() const noexcept { return m_isOpen; }
    inline const ImGuiWindow* GetImGuiWindow() const noexcept;

private:
    void CheckActiveUpdate() noexcept;
    void CheckChangeWindow(const ImGuiWindow* window);
    void IgnoreMouseClick();
    void SetupWindowAppearing() noexcept;
    void ShowStatusBar();

    IRenderer* m_renderer;
    string m_name{};
    ImGuiWindow* m_window{ nullptr };
    unique_ptr<UIComponent> m_panel;
    unique_ptr<ComponentController> m_controller;

    ImTextureID m_textureID{};
    bool m_isOpen{ false };
    bool m_isActiveUpdate{ false };
    ImVec2 m_size{ 800.f, 600.f };

    static int m_mainWindowIndex;
};
