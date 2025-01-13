#include "pch.h"
#include "ToolSystem.h"
#include "GuiWidget.h"
#include "../Toy/InputManager.h"
#include "../Toy/Utility.h"
#include "../Toy/Config.h"
#include "MainWindow/MainWindow.h"
#include "Menu/MenuBar.h"
#include "Dialog.h"
#include "Config.h"

ToolSystem::~ToolSystem() = default;
ToolSystem::ToolSystem(IRenderer* renderer) :
    m_renderer{ renderer },
    m_config{ make_unique<Tool::Config>() }
{
    m_menuBar = make_unique<MenuBar>(this);
    m_renderer->AddImguiComponent(this);
}

void ToolSystem::SetMainWindow(unique_ptr<MainWindow> mainWindow) noexcept
{
    m_mainWindows.emplace_back(move(mainWindow));
}

MainWindow* ToolSystem::GetFocusMainWindow() const noexcept
{
    auto it = ranges::max_element(
        m_mainWindows,
        [](const auto& rhs, const auto& lhs) {
            const ImGuiWindow* wRhs = rhs->GetImGuiWindow();
            const ImGuiWindow* wlhs = lhs->GetImGuiWindow();
            return wRhs->FocusOrder < wlhs->FocusOrder;
        });

    //auto it = ranges::find_if(m_mainWindows, [](const auto& wnd) {
       // return wnd->IsFocus();
        //});

    if (it == m_mainWindows.end())
        return nullptr;

    return it->get();
}

void ToolSystem::Update(const DX::StepTimer* timer, const InputManager& inputManager)
{
    m_menuBar->Update();

    erase_if(m_mainWindows, [](auto& wnd) { return !wnd->IsOpen(); });
    ranges::for_each(m_mainWindows, [timer, &inputManager](const auto& wnd) {
        wnd->Update(timer, inputManager);
        });
}

void ToolSystem::Render(ImGuiIO* io)
{
    Tool::Dialog::Render();
    m_menuBar->Render();

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