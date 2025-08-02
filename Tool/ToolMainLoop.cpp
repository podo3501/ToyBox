#include "pch.h"
#include "ToolMainLoop.h"
#include "ToolSystem.h"

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

ToolMainLoop::~ToolMainLoop() = default;
ToolMainLoop::ToolMainLoop(Window* window, IRenderer* renderer) :
    ::MainLoop(window, renderer),
    m_window{ window },
    m_renderer{ renderer }
{}

bool ToolMainLoop::InitializeDerived()
{
    m_toolSystem = make_unique<ToolSystem>(m_renderer);

    return true;
}

bool ToolMainLoop::LoadResources()
{
    return true;
}

bool ToolMainLoop::PostLoaded(ITextureController* texController)
{
    return true;
}

void ToolMainLoop::Update(const DX::StepTimer& timer)
{
    PIXBeginEvent(PIX_COLOR_DEFAULT, L"Update");

    UNREFERENCED_PARAMETER(timer);
    m_toolSystem->Update(timer);

    PIXEndEvent();
}