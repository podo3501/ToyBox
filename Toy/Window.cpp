#include "pch.h"
#include "Window.h"
#include "Game.h"
#include "Utility.h"

LPCWSTR g_szAppName = L"Toy";

void ExitGame() noexcept;

//혹시나 있을 큐 안에 메세지 내용을 비운다.
void ClearWindowMessageQueue()
{
    MSG msg;
    while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {}
}

Window::Window()
{
    AddWndProcListener([wnd = this](HWND w, UINT m, WPARAM wp, LPARAM lp)->LRESULT {
        return wnd->MsgProc(w, m, wp, lp); });
}

Window::~Window()
{
    m_wndProcListeners.clear();
    DestroyWindow(m_wnd);
    UnregisterClass(m_className.c_str(), m_hInstance);
    ClearWindowMessageQueue();
}

bool Window::Create(HINSTANCE hInstance, int nCmdShow, const Game* game, RECT& rc, HWND& hwnd)
{
    ReturnIfFalse(WindowRegisterClass(hInstance, m_className));
    ReturnIfFalse(CreateGameWindow(hInstance, game, rc, hwnd));

    m_wnd = hwnd;
    m_hInstance = hInstance;

    ShowWindow(hwnd, nCmdShow);
    // TODO: Change nCmdShow to SW_SHOWMAXIMIZED to default to fullscreen.

    GetClientRect(hwnd, &rc);

    return true;
}

Window* gWindow = nullptr;
bool Window::WindowRegisterClass(HINSTANCE hInstance, const std::wstring& className)
{
    gWindow = this;
    //캡쳐가 있으면( [this]하면 될것 같으나 ) WNDPROC으로 변환이 되지 않는다. 그래서 정적변수활용
    auto windowProc = [](HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)->LRESULT {
        return gWindow->WndProc(hwnd, msg, wParam, lParam);
        };

    WNDCLASSEXW wcex = {};
    wcex.cbSize = sizeof(WNDCLASSEXW);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = windowProc;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIconW(hInstance, L"IDI_ICON");
    wcex.hCursor = LoadCursorW(nullptr, IDC_ARROW);
    wcex.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1);
    wcex.lpszClassName = className.c_str();
    wcex.hIconSm = LoadIconW(wcex.hInstance, L"IDI_ICON");
    if (!RegisterClassExW(&wcex))
        return false;

    return true;
}

bool Window::CreateGameWindow(HINSTANCE hInstance, const Game* game, RECT& rc, HWND& hwnd)
{
    int w, h;
    game->GetDefaultSize(w, h);

    rc = { 0, 0, static_cast<LONG>(w), static_cast<LONG>(h) };

    AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);

    hwnd = CreateWindowExW(0, L"ToyWindowClass", g_szAppName, WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left, rc.bottom - rc.top,
        nullptr, nullptr, hInstance,
        const_cast<Game*>(game));
    // TODO: Change to CreateWindowExW(WS_EX_TOPMOST, L"ToyWindowClass", g_szAppName, WS_POPUP,
    // to default to fullscreen.
    if (!hwnd) return false;

    return true;
}

// Windows procedure
LRESULT CALLBACK Window::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    for (auto wndProcListener : m_wndProcListeners)
    {
        LRESULT result{ 0 };
        result = wndProcListener(hWnd, message, wParam, lParam);
        if (result != 0)
            return result;
    }

    return DefWindowProc(hWnd, message, wParam, lParam);
}

LRESULT CALLBACK Window::MsgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static bool s_in_sizemove = false;
    static bool s_in_suspend = false;
    static bool s_minimized = false;
    static bool s_fullscreen = false;
    // TODO: Set s_fullscreen to true if defaulting to fullscreen.

    auto game = reinterpret_cast<Game*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));

    switch (message)
    {
    case WM_CREATE:
        if (lParam)
        {
            auto params = reinterpret_cast<LPCREATESTRUCTW>(lParam);
            SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(params->lpCreateParams));
        }
        break;

    case WM_PAINT:
        if (s_in_sizemove && game)
        {
            game->Tick();
        }
        else
        {
            PAINTSTRUCT ps;
            std::ignore = BeginPaint(hWnd, &ps);
            EndPaint(hWnd, &ps);
        }
        break;

    case WM_DISPLAYCHANGE:
        if (game)
        {
            game->OnDisplayChange();
        }
        break;

    case WM_MOVE:
        if (game)
        {
            game->OnWindowMoved();
        }
        break;

    case WM_SIZE:
        if (wParam == SIZE_MINIMIZED)
        {
            if (!s_minimized)
            {
                s_minimized = true;
                if (!s_in_suspend && game)
                    game->OnSuspending();
                s_in_suspend = true;
            }
        }
        else if (s_minimized)
        {
            s_minimized = false;
            if (s_in_suspend && game)
                game->OnResuming();
            s_in_suspend = false;
        }
        else if (!s_in_sizemove && game)
        {
            game->OnWindowSizeChanged(LOWORD(lParam), HIWORD(lParam));
        }
        break;

    case WM_ENTERSIZEMOVE:
        s_in_sizemove = true;
        break;

    case WM_EXITSIZEMOVE:
        s_in_sizemove = false;
        if (game)
        {
            RECT rc;
            GetClientRect(hWnd, &rc);

            game->OnWindowSizeChanged(rc.right - rc.left, rc.bottom - rc.top);
        }
        break;

    case WM_GETMINMAXINFO:
        if (lParam)
        {
            auto info = reinterpret_cast<MINMAXINFO*>(lParam);
            info->ptMinTrackSize.x = 320;
            info->ptMinTrackSize.y = 200;
        }
        break;

    case WM_ACTIVATEAPP:
        if (game)
        {
            if (wParam)
            {
                game->OnActivated();
            }
            else
            {
                game->OnDeactivated();
            }
        }
        break;

    case WM_POWERBROADCAST:
        switch (wParam)
        {
        case PBT_APMQUERYSUSPEND:
            if (!s_in_suspend && game)
                game->OnSuspending();
            s_in_suspend = true;
            return TRUE;

        case PBT_APMRESUMESUSPEND:
            if (!s_minimized)
            {
                if (s_in_suspend && game)
                    game->OnResuming();
                s_in_suspend = false;
            }
            return TRUE;
        }
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    case WM_SYSKEYDOWN:
        if (wParam == VK_RETURN && (lParam & 0x60000000) == 0x20000000)
        {
            // Implements the classic ALT+ENTER fullscreen toggle
            if (s_fullscreen)
            {
                SetWindowLongPtr(hWnd, GWL_STYLE, WS_OVERLAPPEDWINDOW);
                SetWindowLongPtr(hWnd, GWL_EXSTYLE, 0);

                int width = 800;
                int height = 600;
                if (game)
                    game->GetDefaultSize(width, height);

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

            s_fullscreen = !s_fullscreen;
        }
        break;

    case WM_MENUCHAR:
        // A menu is active and the user presses a key that does not correspond
        // to any mnemonic or accelerator key. Ignore so we don't produce an error beep.
        return MAKELRESULT(0, MNC_CLOSE);
    }
    
    return 0;
}

// Exit helper
void ExitGame() noexcept
{
    PostQuitMessage(0);
}

