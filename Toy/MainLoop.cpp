#include "pch.h"
#include "MainLoop.h"
#include "../Include/IRenderer.h"
#include "Utility.h"
#include "Config.h"
#include "Window.h"
#include "StepTimer.h"
#include "WindowProcedure.h"
#include "HelperClass.h"
#include "InputManager.h"

MainLoop::~MainLoop() = default;
MainLoop::MainLoop(Window* window, IRenderer* renderer) :
    m_window{ window },
    m_renderer{ renderer },
    m_inputManager{ nullptr }
{}

bool MainLoop::Initialize(const wstring& resPath, const Vector2& resolution)
{
    InitializeConfig(resPath, resolution);
    
    ReturnIfFalse(InitializeClass());
    ReturnIfFalse(InitializeDerived());
    AddWinProcListener();

    ReturnIfFalse(LoadResources());

    return true;
}

bool MainLoop::InitializeClass()
{
    m_inputManager = make_unique<InputManager>(m_window->GetHandle());
    m_timer = make_unique<DX::StepTimer>();

    // TODO: Change the timer settings if you want something other than the default variable timestep mode.
    // e.g. for 60 FPS fixed timestep update logic, call:
    /*
    m_timer.SetFixedTimeStep(true);
    m_timer.SetTargetElapsedSeconds(1.0 / 60);
    */

    return true;
}

void MainLoop::AddWinProcListener() noexcept
{
    m_window->AddWndProcListener([mainLoop = this](HWND wnd, UINT msg, WPARAM wp, LPARAM lp)->LRESULT {
        return mainLoop->WndProc(wnd, msg, wp, lp); });
    m_window->AddWndProcListener([](HWND wnd, UINT msg, WPARAM wp, LPARAM lp)->LRESULT {
        return MouseProc(wnd, msg, wp, lp); });
}

int MainLoop::Run()
{
    // Main message loop
    MSG msg = {};
    while (WM_QUIT != msg.message)
    {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else
            Tick();
    }

    return static_cast<int>(msg.wParam);
}

void MainLoop::Tick()
{
    auto timer = m_timer.get();
    m_inputManager->Update();

    m_timer->Tick([&, this]()
        {
            Update(timer, m_inputManager.get());
        });
   
    // Don't try to render anything before the first Update.
    if (m_timer->GetFrameCount() == 0)
        return;

    m_renderer->Draw(); //Scene(Component의 집합)을 렌더링
}

void MainLoop::OnResuming() const
{
    m_timer->ResetElapsedTime();

    m_renderer->OnResuming();
}