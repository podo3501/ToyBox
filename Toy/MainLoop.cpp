#include "pch.h"
#include "MainLoop.h"
#include "Game.h"
#include "GameWindow.h"
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

bool MainLoop::Initialize(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    ReturnIfFalse(XMVerifyCPUSupport())

#ifdef __MINGW32__
    ReturnIfFailed(CoInitializeEx(nullptr, COINITBASE_MULTITHREADED))
#else
    Microsoft::WRL::Wrappers::RoInitializeWrapper initialize(RO_INIT_MULTITHREADED);
    ReturnIfFailed(initialize);
#endif

    m_game = std::make_unique<Game>();
    m_window = std::make_unique<GameWindow>();
    
    HWND hwnd{ 0 };
    RECT rc{};
    ReturnIfFalse(m_window->Create(hInstance, nCmdShow, m_game.get(), rc, hwnd));
    ReturnIfFalse(m_game->Initialize(hwnd, rc.right - rc.left, rc.bottom - rc.top));

    return true;
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
