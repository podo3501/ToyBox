#include "pch.h"
#include "ToolMainLoop.h"
#include "TestImgui.h"
#include "GuiWidget.h"
#include "GuiAppWindow.h"
#include "../Toy/Utility.h"
#include "../Toy/UserInterface/Scene.h"
#include "../Toy/UserInterface/UIType.h"
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
    m_testImgui{ make_unique<TestImgui>(renderer) },
    m_toolScene{ make_unique<Scene>(renderer) }
{
    m_renderer->AddLoadScene(m_toolScene.get());
    m_renderer->AddRenderScene(m_toolScene.get());
}

bool ToolMainLoop::InitializeDerived()
{
    return true;
}

bool ToolMainLoop::LoadResources()
{
    //다이얼로그를 열면 0.1씩 벗어나야 하고, 최종창에는 벗어나지 않아야 한다.
    unique_ptr<Dialog> dialog = make_unique<Dialog>();
    ReturnIfFalse(dialog->SetResources(L"UI/Data/Dialog.json"));

    m_toolScene->AddComponent({ 0.f, 0.f }, move(dialog));

    return true;
}

bool ToolMainLoop::SetDatas(IGetValue* getValue)
{
    m_guiAppWindow = make_unique<GuiAppWindow>(m_renderer);

    auto cloneScene = m_toolScene->Clone();
    ReturnIfFalse(m_guiAppWindow->Create(move(cloneScene), { 400, 300 }));

    return true;
}

void ToolMainLoop::Update(const DX::StepTimer* timer, const Mouse::ButtonStateTracker* mouseTracker)
{
    PIXBeginEvent(PIX_COLOR_DEFAULT, L"Update");

    UNREFERENCED_PARAMETER(timer);
    //float elapsedTime = float(timer->GetElapsedSeconds());

    m_toolScene->Update(mouseTracker);

    m_guiAppWindow->Update();

    PIXEndEvent();
}