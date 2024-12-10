#pragma once
#include "../Include/IRenderer.h"

namespace DX
{
    class StepTimer;
}

class UIComponent;
class InputManager;
class MouseTracker;
class ComponentTooltip;
class ComponentPopup;
class ComponentWindow;

class MainWindow : public IImguiComponent
{
public:
    MainWindow(IRenderer* renderer);
    ~MainWindow();

    virtual void Render(ImGuiIO* io) override;

    bool CreateScene(const XMUINT2& size);
    bool CreateScene(const wstring& filename);
    bool SaveScene(const wstring& filename);
    bool IsFocus() const noexcept;
    const wstring& GetSaveFilename() const noexcept;
    void Update(const DX::StepTimer* timer, InputManager* inputManager);
    void ChangeWindowSize(const ImVec2& size);

    inline bool IsOpen() const noexcept { return m_isOpen; }
    inline const ImGuiWindow* GetImGuiWindow() const noexcept;

private:
    void CheckChangeWindow(const ImGuiWindow* window, const MouseTracker* mouseTracker);
    void CheckAddComponent(const MouseTracker* mouseTracker) noexcept;
    void CheckSelectedComponent(InputManager* inputManager);
    void RenderMain();

    IRenderer* m_renderer;
    unique_ptr<UIComponent> m_panel;
    unique_ptr<ComponentPopup> m_popup;
    unique_ptr<ComponentWindow> m_comWindow;
    unique_ptr<ComponentTooltip> m_tooltip;

    string m_name{};
    ImTextureID m_textureID{};
    bool m_isOpen{ false };
    ImVec2 m_size{ 800.f, 600.f };
};
