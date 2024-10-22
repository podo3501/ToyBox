#include "pch.h"
#include "ToolMainLoop.h"
#include "TestImgui.h"
#include "../Toy/Utility.h"
#include "../Toy/UserInterface/UIType.h"
#include "../Toy/UserInterface/Dialog.h"
#include "../Toy/UserInterface/UILayout.h"

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
    m_dialog = make_unique<Dialog>();

    return true;
}

bool ToolMainLoop::LoadResources(const wstring& resPath)
{
    AddImguiItem(m_testImgui.get());

    UILayout layout({ 0, 0, 220, 190 }, { 0.f, 0.f }, Origin::LeftTop);
    ImageSource dialogSource{
        L"UI/Blue/button_square_header_large_square_screws.png", {
            { 0, 0, 30, 36 }, { 30, 0, 4, 36 }, { 34, 0, 30, 36 },
            { 0, 36, 30, 2 }, { 30, 36, 4, 2 }, { 34, 36, 30, 2 },
            { 0, 38, 30, 26 }, { 30, 38, 4, 26 }, { 34, 38, 30, 26 }
        }
    };

    m_dialog->SetImage(resPath, dialogSource, layout);
    AddRenderItem(m_dialog.get());

    
    //ImTextureID texID = renderer->GetRenderTexture({ 220, 190 }, m_dialog->Render);
    //igWidget = make_unique<IGWidget>(texID);

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

    m_dialog->Update(resolution);

    PIXEndEvent();
}