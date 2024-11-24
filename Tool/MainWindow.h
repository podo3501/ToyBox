#pragma once
#include "../Include/IRenderer.h"

class MainWindow : public IImguiComponent
{
public:
    MainWindow(IRenderer* renderer);
    ~MainWindow();
    bool CreateScene(const wstring& filename);

    //IImguiComponent
    virtual void Update() override;
    virtual void Render(ImGuiIO* io) override;

private:
    IRenderer* m_renderer;
    unique_ptr<IRenderScene> m_scene;
};
