#include "pch.h"
#include "ToolSystem.h"
#include "GuiWidget.h"
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

void ToolSystem::Update(const DX::StepTimer& timer)
{
    m_menuBar->Update();

    erase_if(m_mainWindows, [](auto& wnd) { return !wnd->IsOpen(); });
    ranges::for_each(m_mainWindows, [&timer](const auto& wnd) {
        wnd->Update(timer);
        });
}

void ToolSystem::Render(ImGuiIO* io)
{
    Tool::Dialog::Render();
    m_menuBar->Render();

    return;
}