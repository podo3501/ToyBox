#include "pch.h"
#include "MainLoop.h"
#include "../Include/IRenderer.h"
#include "UserInterface/UIType.h"
#include "Window.h"
#include "Utility.h"
#include "UserInterface/Button.h"
#include "UserInterface/Dialog.h"
#include "MouseProcedure.h"
#include "StepTimer.h"

#ifdef __clang__
#pragma clang diagnostic ignored "-Wcovered-switch-default"
#pragma clang diagnostic ignored "-Wswitch-enum"
#endif

#pragma warning(disable : 4061)

#ifdef USING_D3D12_AGILITY_SDK
extern "C"
{
    // Used to enable the "Agility SDK" components
    __declspec(dllexport) extern const UINT D3D12SDKVersion = D3D12_SDK_VERSION;
    __declspec(dllexport) extern const char* D3D12SDKPath = u8".\\D3D12\\";
}
#endif

LRESULT MainLoop::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_ACTIVATEAPP:
        if (wParam)
            m_renderer->OnActivated();
        else
            m_renderer->OnDeactivated();
        break;

    case WM_DISPLAYCHANGE:
        m_renderer->OnDisplayChange();
        break;

    case WM_MOVE:
        m_renderer->OnWindowMoved();
        break;

    case WM_PAINT:
        if (m_sizemove)
            Tick();
        else
        {
            PAINTSTRUCT ps;
            ignore = BeginPaint(hWnd, &ps);
            EndPaint(hWnd, &ps);
        }
        break;

    case WM_SIZE:
        if (wParam == SIZE_MINIMIZED)
        {
            if (!m_minimized)
            {
                m_minimized = true;
                if (!m_suspend)
                    m_renderer->OnSuspending();
                m_suspend = true;
            }
        }
        else if (m_minimized)
        {
            m_minimized = false;
            if (m_suspend)
                OnResuming();
            m_suspend = false;
        }
        else //if (!m_sizemove) //창을 변경할때 업데이트 하도록 수정
        {
            int width = LOWORD(lParam);
            int height = HIWORD(lParam);

            RECT rc;
            rc.left = rc.top = 0;
            rc.right = static_cast<LONG>(width);
            rc.bottom = static_cast<LONG>(height);
            m_window->OnWindowSizeChanged(rc);
            m_renderer->OnWindowSizeChanged(LOWORD(lParam), HIWORD(lParam));
        }
        break;

    case WM_ENTERSIZEMOVE:
        m_sizemove = true;
        break;

    case WM_EXITSIZEMOVE:
        m_sizemove = false;
        {
            RECT rc;
            GetClientRect(hWnd, &rc);

            m_window->OnWindowSizeChanged(rc);
            m_renderer->OnWindowSizeChanged(rc.right - rc.left, rc.bottom - rc.top);
        }
        break;

    case WM_POWERBROADCAST:
        switch (wParam)
        {
        case PBT_APMQUERYSUSPEND:
            if (!m_suspend)
                m_renderer->OnSuspending();
            m_suspend = true;
            return TRUE;
        case PBT_APMRESUMESUSPEND:
            if (!m_minimized)
            {
                if (m_suspend)
                    OnResuming();
                m_suspend = false;
            }
            return TRUE;
        }
        break;

    case WM_SYSKEYDOWN:
        if (wParam == VK_RETURN && (lParam & 0x60000000) == 0x20000000)
        {
            // Implements the classic ALT+ENTER fullscreen toggle
            if (m_fullscreen)
            {
                SetWindowLongPtr(hWnd, GWL_STYLE, WS_OVERLAPPEDWINDOW);
                SetWindowLongPtr(hWnd, GWL_EXSTYLE, 0);

                const auto& resolution = m_window->GetOutputSize();
                int width = static_cast<int>(resolution.x);
                int height = static_cast<int>(resolution.y);

                ShowWindow(hWnd, SW_SHOWNORMAL);
                SetWindowPos(hWnd, HWND_TOP, 0, 0, width, height, SWP_NOMOVE | SWP_NOZORDER | SWP_FRAMECHANGED);
            }
            else
            {
                SetWindowLongPtr(hWnd, GWL_STYLE, WS_POPUP);
                SetWindowLongPtr(hWnd, GWL_EXSTYLE, WS_EX_TOPMOST);

                SetWindowPos(hWnd, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);
                ShowWindow(hWnd, SW_SHOWMAXIMIZED);
            }

            m_fullscreen = !m_fullscreen;
        }
        break;
    }

    return 0;
}

MainLoop::MainLoop() :
    m_renderer{ nullptr } {}
MainLoop::~MainLoop() = default;

bool MainLoop::Initialize(HINSTANCE hInstance, const wstring& resPath, int nCmdShow)
{
    m_resourcePath = resPath;

    ReturnIfFalse(XMVerifyCPUSupport());
    ReturnIfFalse(InitializeClass(hInstance, nCmdShow));

    AddWinProcListener();

    return true;
}

bool MainLoop::InitializeClass(HINSTANCE hInstance, int nCmdShow)
{
    m_window = make_unique<Window>();
    m_mouse = make_unique<Mouse>();
    m_timer = make_unique<DX::StepTimer>();

    HWND hwnd{ 0 };
    RECT rc{0, 0, 800, 600};
    ReturnIfFalse(m_window->Create(hInstance, nCmdShow, rc, hwnd));
    const auto& outputSize = m_window->GetOutputSize();
    m_renderer = CreateRenderer(hwnd, static_cast<int>(outputSize.x), static_cast<int>(outputSize.y));
    if (m_renderer == nullptr)
        return false;

    m_button = make_unique<Button>();
    m_button2 = make_unique<Button>();
    vector<ImageSource> normal
    {
        { L"UI/Blue/bar_square_large_l.png" },
        { L"UI/Blue/bar_square_large_m.png" },
        { L"UI/Blue/bar_square_large_r.png" },
    };
    vector<ImageSource> hover
    {
        { L"UI/Red/bar_square_large_l.png" },
        { L"UI/Red/bar_square_large_m.png" },
        { L"UI/Red/bar_square_large_r.png" },
    };
    vector<ImageSource> pressed
    {
        { L"UI/Gray/bar_square_large_l.png" },
        { L"UI/Gray/bar_square_large_m.png" },
        { L"UI/Gray/bar_square_large_r.png" },
    };
    Rectangle area{ 0, 0, 180, 48 };
    m_button->SetImage(m_resourcePath, normal, hover, pressed, area, SimpleMath::Vector2{ 0.5f, 0.5f }, Origin::Center);
    m_button2->SetImage(m_resourcePath, normal, hover, pressed, area, SimpleMath::Vector2{ 0.5f, 0.4f }, Origin::Center);

    m_mouse->SetWindow(hwnd);

    m_renderer->AddRenderItem(m_button.get());
    m_renderer->AddRenderItem(m_button2.get());

    m_dialog = make_unique<Dialog>();
    Rectangle dialongArea{ 0, 0, 220, 190 };
    XMFLOAT2 pos{ 0.65f, 0.45f };
    ImageSource dialogSource{
        L"UI/Blue/button_square_header_large_square_screws.png", {
            { 0, 0, 30, 36 }, { 30, 0, 4, 36 }, { 34, 0, 30, 36 },
            { 0, 36, 30, 2 }, { 30, 36, 4, 2 }, { 34, 36, 30, 2 },
            { 0, 38, 30, 26 }, { 30, 38, 4, 26 }, { 34, 38, 30, 26 }
        }
    };

    m_dialog->SetImage(m_resourcePath, dialogSource, dialongArea, pos, Origin::Center);
    m_renderer->AddRenderItem(m_dialog.get());

    m_closeButton = std::make_unique<Button>();

    vector<ImageSource> normal2{ { L"UI/Blue/check_square_color_cross.png" } };
    vector<ImageSource> hover2{ { L"UI/Blue/check_square_grey_cross.png" } };
    vector<ImageSource> pressed2{ { L"UI/Gray/check_square_grey_cross.png" } };

    Rectangle area2{ 0, 0, 32, 32 };
    XMFLOAT2 pos2{ 0.2f, 0.2f };
    m_closeButton->SetImage(m_resourcePath, normal2, hover2, pressed2, area2, pos2, Origin::Center);
    m_renderer->AddRenderItem(m_closeButton.get());
    
    m_renderer->LoadResources();

    // TODO: Change the timer settings if you want something other than the default variable timestep mode.
    // e.g. for 60 FPS fixed timestep update logic, call:
    /*
    m_timer.SetFixedTimeStep(true);
    m_timer.SetTargetElapsedSeconds(1.0 / 60);
    */

    return true;
}

void MainLoop::AddWinProcListener()
{
    m_window->AddWndProcListener([mainLoop = this](HWND wnd, UINT msg, WPARAM wp, LPARAM lp)->LRESULT {
        return mainLoop->WndProc(wnd, msg, wp, lp); });
    m_window->AddWndProcListener([](HWND wnd, UINT msg, WPARAM wp, LPARAM lp)->LRESULT {
        return MouseProc(wnd, msg, wp, lp); });
}

Mouse::ButtonStateTracker mouseTracker;
void MainLoop::Update(DX::StepTimer* timer)
{
    PIXBeginEvent(PIX_COLOR_DEFAULT, L"Update");

    UNREFERENCED_PARAMETER(timer);
    //float elapsedTime = float(timer->GetElapsedSeconds());

    mouseTracker.Update(m_mouse->GetState());
    m_button->Update(m_window->GetOutputSize(), mouseTracker);
    m_button2->Update(m_window->GetOutputSize(), mouseTracker);
    m_closeButton->Update(m_window->GetOutputSize(), mouseTracker);
    m_dialog->Update(m_window->GetOutputSize());

    PIXEndEvent();
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

    m_timer->Tick([&]()
        {
            Update(timer);
        });

    // Don't try to render anything before the first Update.
    if (m_timer->GetFrameCount() != 0)
    {
        m_renderer->Draw();
    }
}

void MainLoop::OnResuming()
{
    m_timer->ResetElapsedTime();

    m_renderer->OnResuming();
}
