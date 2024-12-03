#include "pch.h"
#include "MainLoop.h"
#include "../Include/IRenderer.h"
#include "Utility.h"
#include "Window.h"
#include "StepTimer.h"
#include "WindowProcedure.h"
#include "Config.h"
#include "HelperClass.h"

MainLoop::~MainLoop() = default;
MainLoop::MainLoop(Window* window, IRenderer* renderer) :
    m_window{ window },
    m_renderer{ renderer }
{}

bool MainLoop::Initialize(const wstring& resPath, const Vector2& resolution)
{
    InitializeConfig(resPath, resolution);
    
    ReturnIfFalse(InitializeClass());
    ReturnIfFalse(InitializeDerived());
    AddWinProcListener();

    ReturnIfFalse(LoadResources());
    m_renderer->LoadComponents();

    return true;
}

bool MainLoop::InitializeClass()
{
    m_mouse = make_unique<Mouse>();
    m_timer = make_unique<DX::StepTimer>();
    m_mouse->SetWindow(m_window->GetHandle());

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
    static MouseTracker customMouseTracker;
    customMouseTracker.Update(m_mouse->GetState());

    m_timer->Tick([&]()
        {
            Update(timer, &customMouseTracker);
        });
   
    // Don't try to render anything before the first Update.
    if (m_timer->GetFrameCount() == 0)
        return;

    m_renderer->Draw(); //Scene(Component의 집합)을 랜더링
}

void MainLoop::OnResuming() const
{
    m_timer->ResetElapsedTime();

    m_renderer->OnResuming();
}