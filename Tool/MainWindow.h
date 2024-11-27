#pragma once
#include "../Include/IRenderer.h"

namespace DX
{
    class StepTimer;
}

class CustomButtonStateTracker;
class Scene;

class MainWindow : public IImguiComponent
{
public:
    MainWindow(IRenderer* renderer);
    ~MainWindow();
    bool CreateScene(const wstring& filename);
    void Update(const DX::StepTimer* timer, CustomButtonStateTracker* mouseTracker);
    //IImguiComponent
    //virtual void Update() override;
    virtual void Render(ImGuiIO* io) override;

private:
    IRenderer* m_renderer;
    string m_name{};
    unique_ptr<Scene> m_scene;
    ImTextureID m_textureID{};
    bool m_visible{ false };
};
