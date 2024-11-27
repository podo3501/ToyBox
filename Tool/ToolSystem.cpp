#include "pch.h"
#include "ToolSystem.h"
#include "GuiWidget.h"
#include "../Include/IRenderScene.h"
#include "../Toy/UserInterface/BGImage.h"
#include "../Toy/UserInterface/Dialog.h"
#include "../Toy/Utility.h"
#include "../Toy/Config.h"
#include "MainWindow.h"
#include "MainMenuBar.h"
#include "Popup.h"

using namespace Tool;

ToolSystem::~ToolSystem() = default;
ToolSystem::ToolSystem(IRenderer* renderer) :
    m_renderer{ renderer },
    m_popup{ make_unique<Popup>() },
    m_component{ nullptr }
{
    m_mainMenuBar = make_unique<MainMenuBar>(this, m_popup.get());
    m_renderer->AddImguiComponent(this);
}

void ToolSystem::SetMainWindow(shared_ptr<MainWindow> mainWindow) noexcept
{
    m_mainWindow = mainWindow; 
}

bool ToolSystem::Create(unique_ptr<IRenderScene> scene, const XMUINT2& size)
{
    //ReturnIfFalse(m_renderer->CreateRenderTexture(size, scene.get(), m_textureID));
    //auto dialog = scene->GetComponent("Dialog_clone");
    //m_component = dialog->GetComponent("BGImage_clone");
    //m_size = size;

    ////임시로 bgImage가 선택되었다고 가정한다.
    //m_guiWidget = make_unique<GuiWidget>(m_renderer);
    //unique_ptr<UIComponent> clone = m_component->Clone();
    //
    //ReturnIfFalse(m_guiWidget->Create(move(clone)));
    
    //m_scene = move(scene);
    return true;
}

void ToolSystem::Update(const DX::StepTimer* timer, CustomButtonStateTracker* mouseTracker)
{
    m_mainMenuBar->Update();
    if (m_mainWindow)
        m_mainWindow->Update(timer, mouseTracker);
    //m_guiWidget->Update();
    //m_scene->Update(nullptr);
}

void ToolSystem::Render(ImGuiIO* io)
{
    m_popup->Render();
    m_mainMenuBar->Excute();

    return;

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin("App Window", &m_visible, ImGuiWindowFlags_AlwaysAutoResize);
    ImGui::PopStyleVar();   //윈도우 스타일을 지정한다.

    static bool bPicking{ false };
    ImVec2 curWindowPos = ImGui::GetWindowPos();
    ImVec2 startPos{ curWindowPos.x + 0.f, curWindowPos.y + 0.f + ImGui::GetFrameHeight() };
    if (/*io->MouseDown[0] && */io->MouseReleased[0])
    {
        Vector2 mousePos{ io->MousePos.x, io->MousePos.y };

        Vector2 realMousePos{};
        realMousePos.x = mousePos.x - startPos.x;
        realMousePos.y = mousePos.y - startPos.y;

        //클릭했을때 픽킹이 되었는지 확인
        //if (m_component->IsPicking(realMousePos))
        //    m_guiWidget->SetVisible(true);
    }

    ImGui::Image(m_textureID, { static_cast<float>(m_size.x), static_cast<float>(m_size.y) });
    //ImGui::Image(m_textureID, { static_cast<float>(800), static_cast<float>(600) });
    ImGui::End();
    return;
}