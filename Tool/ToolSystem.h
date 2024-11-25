#pragma once
#include "../Include/IRenderer.h"

struct IRenderScene;
class UIComponent;
class MainWindow;
class GuiWidget;
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

    //IImguiComponent
    virtual void Update() override;
    virtual void Render(ImGuiIO* io) override;

private:
    IRenderer* m_renderer;
    unique_ptr<Tool::MainMenuBar> m_mainMenuBar;
    unique_ptr<Tool::Popup> m_popup;    //modal 창을 띄워주는 클래스(다른것들도 들어가서 나중에 확장될 가능성이 있음)
    unique_ptr<IRenderScene> m_scene;
    UIComponent* m_component;
    ImTextureID m_textureID{};
    bool m_visible{ true };

    XMUINT2 m_size{};

    unique_ptr<MainWindow> m_mainWindow;
    unique_ptr<GuiWidget> m_guiWidget;
};
