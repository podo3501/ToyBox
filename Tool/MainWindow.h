#pragma once
#include "../Include/IRenderer.h"

namespace DX
{
    class StepTimer;
}

namespace Tool
{
    class Popup;
}

class MouseTracker;
class UIComponent;

class MainWindow : public IImguiComponent
{
public:
    MainWindow(IRenderer* renderer);
    ~MainWindow();
    bool CreateScene(const XMUINT2& size);
    bool CreateScene(const wstring& filename);
    bool SaveScene(const wstring& filename);
    bool IsFocus() const noexcept;
    inline bool IsOpen() const noexcept { return m_isOpen; }
    inline const ImGuiWindow* GetImGuiWindow() const noexcept;
    const wstring& GetSaveFilename() const noexcept;
    void Update(const DX::StepTimer* timer, MouseTracker* mouseTracker);
    //IImguiComponent
    //virtual void Update() override;
    virtual void Render(ImGuiIO* io) override;
    void ChangeWindowSize(const ImVec2& size);

private:
    void CheckChangeWindow(const ImGuiWindow* window, MouseTracker* mouseTracker);

    IRenderer* m_renderer;
    unique_ptr<Tool::Popup> m_popup;

    string m_name{};
    unique_ptr<UIComponent> m_panel;
    ImTextureID m_textureID{};
    bool m_isOpen{ false };
    ImVec2 m_size{ 800.f, 600.f };
};
