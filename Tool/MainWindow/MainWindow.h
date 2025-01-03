#pragma once
#include "../Include/IRenderer.h"

namespace DX
{
    class StepTimer;
}

class UIComponent;
class InputManager;
class MouseTracker;
class ComponentPopup;
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
    void Update(const DX::StepTimer* timer, InputManager* inputManager);
    void ChangeWindowSize(const ImVec2& size);

    inline bool IsOpen() const noexcept { return m_isOpen; }
    inline const ImGuiWindow* GetImGuiWindow() const noexcept;

private:
    void CheckChangeWindow(const ImGuiWindow* window, const MouseTracker* mouseTracker);
    void CheckAddComponent(const MouseTracker* mouseTracker) noexcept;

    IRenderer* m_renderer;
    ImGuiWindow* m_window{ nullptr };
    unique_ptr<UIComponent> m_panel;
    unique_ptr<ComponentPopup> m_popup;
    unique_ptr<ComponentSelector> m_selector;

    string m_name{};
    ImTextureID m_textureID{};
    bool m_isOpen{ false };
    ImVec2 m_size{ 800.f, 600.f };
};
