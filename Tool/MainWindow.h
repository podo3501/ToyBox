#pragma once
#include "../Include/IRenderer.h"

namespace DX
{
    class StepTimer;
}

class Scene;

class MainWindow : public IImguiComponent, public enable_shared_from_this<MainWindow>
{
public:
    MainWindow(IRenderer* renderer);
    ~MainWindow();
    bool CreateScene(const wstring& filename);
    void Update(const DX::StepTimer* timer, const Mouse::ButtonStateTracker* mouseTracker);
    //IImguiComponent
    //virtual void Update() override;
    virtual void Render(ImGuiIO* io) override;

private:
    IRenderer* m_renderer;
    unique_ptr<Scene> m_scene;
    ImTextureID m_textureID{};
    bool m_visible{ false };
};
