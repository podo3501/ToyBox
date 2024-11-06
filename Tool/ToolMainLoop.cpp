#include "pch.h"
#include "ToolMainLoop.h"
#include "TestImgui.h"
#include "GuiWidget.h"
#include "GuiAppWindow.h"
#include "../Toy/Utility.h"
#include "../Toy/UserInterface/UIType.h"
#include "../Toy/UserInterface/BGImage.h"
#include "../Toy/UserInterface/UILayout.h"
#include "../Toy/UserInterface/JsonHelper.h"
#include "../Toy/UserInterface/Dialog.h"

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
    m_renderer{ renderer },
    m_testImgui{ make_unique<TestImgui>(renderer) }
{}

bool ToolMainLoop::InitializeDerived()
{
    m_dialog = make_unique<Dialog>();

    return true;
}

bool ToolMainLoop::LoadResources()
{
    //다이얼로그를 열면 0.1씩 벗어나야 하고, 최종창에는 벗어나지 않아야 한다.
    ReturnIfFalse(m_dialog->SetResources(L"UI/Data/Dialog.json"));

    m_renderer->AddLoadResource(m_dialog.get());
    m_renderer->AddRenderItem(m_dialog.get());

    m_guiAppWindow = make_unique<GuiAppWindow>(m_renderer);
    ReturnIfFalse(m_guiAppWindow->Create(m_dialog.get(), { 400, 300 }));
    
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

    m_dialog->Update(Vector2{});

    PIXEndEvent();
}