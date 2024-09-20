#include "pch.h"
#include "MainLoop.h"
#include "Game.h"
#include "Window.h"
#include "Utility.h"
#include "Button.h"

using namespace DirectX;

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
    UNREFERENCED_PARAMETER(hWnd);

    switch (message)
    {
    case WM_ACTIVATEAPP:
    case WM_ACTIVATE:
    case WM_INPUT:
    case WM_MOUSEMOVE:
    case WM_LBUTTONDOWN:
    case WM_LBUTTONUP:
    case WM_RBUTTONDOWN:
    case WM_RBUTTONUP:
    case WM_MBUTTONDOWN:
    case WM_MBUTTONUP:
    case WM_MOUSEWHEEL:
    case WM_XBUTTONDOWN:
    case WM_XBUTTONUP:
    case WM_MOUSEHOVER:
        Mouse::ProcessMessage(message, wParam, lParam);
        break;
    }

    return 0;
}

MainLoop::MainLoop() :
    m_game{ nullptr } {}
MainLoop::~MainLoop() = default;

bool MainLoop::Initialize(HINSTANCE hInstance, const std::wstring& resPath, int nCmdShow)
{
    ReturnIfFalse(XMVerifyCPUSupport());
    ReturnIfFalse(InitializeClass(hInstance, resPath, nCmdShow));

    AddWinProcListener();

    return true;
}

bool MainLoop::InitializeClass(HINSTANCE hInstance, const std::wstring& resPath, int nCmdShow)
{
    //com을 생성할때 다중쓰레드로 생성하게끔 초기화 한다. RAII이기 때문에 com을 사용할때 초기화 한다.
#ifdef __MINGW32__
    ReturnIfFailed(CoInitializeEx(nullptr, COINITBASE_MULTITHREADED))
#else
    Microsoft::WRL::Wrappers::RoInitializeWrapper initialize(RO_INIT_MULTITHREADED);
    ReturnIfFailed(initialize);
#endif

    m_window = std::make_unique<Window>();
    m_game = std::make_unique<Game>();
    m_mouse = std::make_unique<Mouse>();

    HWND hwnd{ 0 };
    RECT rc{};
    ReturnIfFalse(m_window->Create(hInstance, nCmdShow, rc, hwnd));
    int width = static_cast<int>(rc.right - rc.left);
    int height = static_cast<int>(rc.bottom - rc.top);
    m_button = std::make_unique<Button>(resPath, width / 2, height / 2);
    m_game->SetRenderItem(m_button.get());
    m_mouse->SetWindow(hwnd);

    //RenderItem을 다 등록시킨후 initialize 한다.
    ReturnIfFalse(m_game->Initialize(hwnd, width, height));

    return true;
}

void MainLoop::AddWinProcListener()
{
    m_window->AddWndProcListener([&g = m_game](HWND wnd, UINT msg, WPARAM wp, LPARAM lp)->LRESULT {
        return g->WndProc(wnd, msg, wp, lp); });
    m_window->AddWndProcListener([mainLoop = this](HWND wnd, UINT msg, WPARAM wp, LPARAM lp)->LRESULT {
        return mainLoop->WndProc(wnd, msg, wp, lp); });
}

void MainLoop::Update()
{
    Mouse::State state = m_mouse->GetState();
    m_button->Update(state);
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
        {
            Update();

            m_game->Tick();
        }
    }

    m_game.reset();

    return static_cast<int>(msg.wParam);
}
