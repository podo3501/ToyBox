#pragma once
#include "../Include/IRenderer.h"

struct IRenderScene;
class UIComponent;
class CustomButtonStateTracker;
class MainWindow;
class GuiWidget;

namespace DX
{
    class StepTimer;
}

namespace Tool
{
    class MainMenuBar;
    class Popup;
}

class ToolSystem : public IImguiComponent
{
public:
    ~ToolSystem();
    ToolSystem(IRenderer* renderer);

    bool Create(unique_ptr<IRenderScene> scene, const XMUINT2& size);
    void Update(const DX::StepTimer* timer, CustomButtonStateTracker* mouseTracker);

    //IImguiComponent
    //virtual void Update() override;
    virtual void Render(ImGuiIO* io) override;

    void SetMainWindow(shared_ptr<MainWindow> mainWindow) noexcept;
    IRenderer* GetRenderer() const noexcept { return m_renderer; }

private:
    IRenderer* m_renderer;
    unique_ptr<Tool::MainMenuBar> m_mainMenuBar;
    unique_ptr<Tool::Popup> m_popup;    //modal â�� ����ִ� Ŭ����(�ٸ��͵鵵 ���� ���߿� Ȯ��� ���ɼ��� ����)
    unique_ptr<IRenderScene> m_scene;
    UIComponent* m_component;
    ImTextureID m_textureID{};
    bool m_visible{ true };

    XMUINT2 m_size{};

    shared_ptr<MainWindow> m_mainWindow;
    unique_ptr<GuiWidget> m_guiWidget;
};
