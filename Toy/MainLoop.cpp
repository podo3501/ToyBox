#include "pch.h"
#include "MainLoop.h"
#include "Game.h"
#include "Window.h"
#include "Utility.h"

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

    m_game = std::make_unique<Game>(resPath);
    m_window = std::make_unique<Window>();

    HWND hwnd{ 0 };
    RECT rc{};
    ReturnIfFalse(m_window->Create(hInstance, nCmdShow, rc, hwnd));
    ReturnIfFalse(m_game->Initialize(hwnd, rc.right - rc.left, rc.bottom - rc.top));

    return true;
}

void MainLoop::AddWinProcListener()
{
    m_window->AddWndProcListener([&g = m_game](HWND wnd, UINT msg, WPARAM wp, LPARAM lp)->LRESULT {
        return g->WndProc(wnd, msg, wp, lp); });
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
            m_game->Tick();
        }
    }

    m_game.reset();

    return static_cast<int>(msg.wParam);
}
