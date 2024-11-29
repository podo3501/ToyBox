#pragma once
#include "../Include/IRenderer.h"

namespace DX
{
    class StepTimer;
}

class MouseTracker;
class Scene;

class MainWindow : public IImguiComponent
{
public:
    MainWindow(IRenderer* renderer);
    ~MainWindow();
    bool CreateScene(const wstring& filename);
    bool SaveScene() const;
    bool IsFocus() const noexcept;
    inline bool IsOpen() const noexcept { return m_isOpen; }
    inline const ImGuiWindow* GetImGuiWindow() const noexcept;
    void Update(const DX::StepTimer* timer, MouseTracker* mouseTracker);
    //IImguiComponent
    //virtual void Update() override;
    virtual void Render(ImGuiIO* io) override;

private:
    IRenderer* m_renderer;
    string m_name{};
    unique_ptr<Scene> m_scene;
    ImTextureID m_textureID{};
    wstring m_filename{};
    bool m_isOpen{ false };
};
