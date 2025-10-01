#include "pch.h"
#include "MainLoop.h"
#include "IRenderer.h"
#include "../Shared/Profiler.h"
#include "GameConfig.h"
#include "Window.h"
#include "WindowProcedure.h"
#include "InputManager.h"

MainLoop::~MainLoop()
{
    TracyShutdownProfiler();
}

MainLoop::MainLoop(Window* window, IRenderer* renderer) :
    m_window{ window },
    m_renderer{ renderer }
{
    TracyStartupProfiler();
    InputManager::Initialize(m_window->GetHandle());
}

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

        FrameMark; //Tracy측정할때 한 프레임이 끝났다는 것을 마크하는 표시
    }

    return static_cast<int>(msg.wParam);
}

void MainLoop::Tick()
{
    InputManager::Update();

    m_timer.Tick([&, this]()
        {
            Update(m_timer);
        });
   
    // Don't try to render anything before the first Update.
    if (m_timer.GetFrameCount() == 0)
        return;

    m_renderer->Draw(); //Scene(Component의 집합)을 렌더링
}

void MainLoop::OnResuming()
{
    m_timer.ResetElapsedTime();

    m_renderer->OnResuming();
}