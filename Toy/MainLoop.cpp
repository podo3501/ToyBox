#include "pch.h"
#include "MainLoop.h"
#include "../Include/IRenderer.h"
#include "Utility.h"
#include "Window.h"
#include "StepTimer.h"
#include "MouseProcedure.h"

MainLoop::~MainLoop() = default;
MainLoop::MainLoop()
{}

bool MainLoop::Initialize(HINSTANCE hInstance, const wstring& resPath, int nCmdShow)
{
    m_resourcePath = resPath;

    ReturnIfFalse(XMVerifyCPUSupport());
    ReturnIfFalse(InitializeClass(hInstance, nCmdShow));
    ReturnIfFalse(InitializeDerived());

    AddWinProcListener();

    ReturnIfFalse(LoadResources(m_resourcePath));
    m_renderer->LoadResources();

    ReturnIfFalse(SetDatas(m_renderer->GetValue()));

    return true;
}

bool MainLoop::InitializeClass(HINSTANCE hInstance, int nCmdShow)
{
    m_window = make_unique<Window>();
    m_mouse = make_unique<Mouse>();
    m_timer = make_unique<DX::StepTimer>();

    HWND hwnd{ 0 };
    RECT rc{ 0, 0, 800, 600 };
    ReturnIfFalse(m_window->Create(hInstance, nCmdShow, rc, hwnd));
    const auto& outputSize = m_window->GetOutputSize();
    m_renderer = CreateRenderer(hwnd, static_cast<int>(outputSize.x), static_cast<int>(outputSize.y));
    if (m_renderer == nullptr)
        return false;
    m_mouse->SetWindow(hwnd);

    // TODO: Change the timer settings if you want something other than the default variable timestep mode.
    // e.g. for 60 FPS fixed timestep update logic, call:
    /*
    m_timer.SetFixedTimeStep(true);
    m_timer.SetTargetElapsedSeconds(1.0 / 60);
    */

    return true;
}

void MainLoop::AddRenderItem(IRenderItem* item) noexcept
{
    m_renderer->AddRenderItem(item);
}

void MainLoop::AddWinProcListener()
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
    CleanUp();

    return static_cast<int>(msg.wParam);
}

Mouse::ButtonStateTracker mouseTracker;
void MainLoop::Tick()
{
    auto timer = m_timer.get();
    mouseTracker.Update(m_mouse->GetState());

    m_timer->Tick([&]()
        {
            Update(timer, m_window->GetOutputSize(), &mouseTracker);
        });
   
    // Don't try to render anything before the first Update.
    if (m_timer->GetFrameCount() == 0)
        return;

    Render();
    m_renderer->Draw(); //item으로 등록시킨 애들을 랜더링
}

void MainLoop::OnResuming()
{
    m_timer->ResetElapsedTime();

    m_renderer->OnResuming();
}
