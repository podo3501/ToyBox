#pragma once
#include "../Include/IRenderer.h"

namespace DX
{
    class StepTimer;
}

class UIComponent;
class InputManager;
class MouseTracker;
class FloatingComponent;
class ComponentSelector;

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
    void Update(const DX::StepTimer* timer, const InputManager& inputManager);
    void ChangeWindowSize(const ImVec2& size);

    inline bool IsOpen() const noexcept { return m_isOpen; }
    inline const ImGuiWindow* GetImGuiWindow() const noexcept;

private:
    void CheckChangeWindow(const ImGuiWindow* window, const MouseTracker& mouseTracker);
    bool CheckAttachComponent(const InputManager& inputManager) noexcept;
    bool CheckDetachComponent(const InputManager& inputManager) noexcept;
    void IgnoreMouseClick();
    void SetupWindowAppearing() noexcept;

    IRenderer* m_renderer;
    string m_name{};
    ImGuiWindow* m_window{ nullptr };
    unique_ptr<UIComponent> m_panel;
    unique_ptr<FloatingComponent> m_floater;
    unique_ptr<ComponentSelector> m_selector;

    ImTextureID m_textureID{};
    bool m_isOpen{ false };
    ImVec2 m_size{ 800.f, 600.f };

    static int m_mainWindowIndex;
};
