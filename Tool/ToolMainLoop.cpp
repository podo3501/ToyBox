#include "pch.h"
#include "ToolMainLoop.h"
#include "TestImgui.h"
#include "../Toy/Utility.h"

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
ToolMainLoop::ToolMainLoop() :
    m_testImgui{ make_unique<TestImgui>() }
{}

bool ToolMainLoop::InitializeDerived()
{
    return true;
}

bool ToolMainLoop::LoadResources(const wstring& resPath)
{
    AddImguiItem(m_testImgui.get());

    return true;
}

bool ToolMainLoop::SetDatas(IGetValue* getValue)
{
    return true;
}

void ToolMainLoop::Update(const DX::StepTimer* timer, const Vector2& resolution, const Mouse::ButtonStateTracker* mouseTracker)
{
    PIXBeginEvent(PIX_COLOR_DEFAULT, L"Update");

    UNREFERENCED_PARAMETER(timer);
    //float elapsedTime = float(timer->GetElapsedSeconds());

    PIXEndEvent();
}