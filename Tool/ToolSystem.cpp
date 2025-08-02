#include "pch.h"
#include "ToolSystem.h"
#include "MainWindow/MainWindow.h"
#include "TextureResBinderWindow/TextureResBinderWindow.h"
#include "Menu/MenuBar.h"
#include "Dialog.h"
#include "ToolConfig.h"

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

void ToolSystem::SetTextureWindow(unique_ptr<TextureResBinderWindow> textureWindow) noexcept
{
    m_textureWindows.emplace_back(move(textureWindow));
}

template <typename InnerWindow>
static InnerWindow* GetFocusWindow(const vector<unique_ptr<InnerWindow>>& windows) noexcept
{
    auto it = ranges::max_element(
        windows,
        [](const auto& rhs, const auto& lhs) {
            const ImGuiWindow* wRhs = rhs->GetImGuiWindow();
            const ImGuiWindow* wlhs = lhs->GetImGuiWindow();
            return wRhs->FocusOrder < wlhs->FocusOrder;
        });

    if (it == windows.end())
        return nullptr;

    return it->get();
}

MainWindow* ToolSystem::GetFocusMainWindow() const noexcept
{
    return GetFocusWindow(m_mainWindows);
}

TextureResBinderWindow* ToolSystem::GetFocusTextureResBinderWindow() const noexcept
{
    return GetFocusWindow(m_textureWindows);
}

void ToolSystem::Update(const DX::StepTimer& timer)
{
    m_menuBar->Update();

    erase_if(m_mainWindows, [](auto& wnd) { return !wnd->IsOpen(); });
    ranges::for_each(m_mainWindows, [&timer](const auto& wnd) {
        wnd->Update(timer);
        });

    erase_if(m_textureWindows, [](auto& texWnd) { return !texWnd->IsOpen(); });
    ranges::for_each(m_textureWindows, [](const auto& texWnd) {
        texWnd->Update();
        });
}

void ToolSystem::Render(ImGuiIO* io)
{
    Tool::Dialog::Render();
    m_menuBar->Render();

    return;
}