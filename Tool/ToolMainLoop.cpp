#include "pch.h"
#include "ToolMainLoop.h"
#include "TestImgui.h"
#include "GuiWidget.h"
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
ToolMainLoop::ToolMainLoop(Window* window, IRenderer* renderer) :
    ::MainLoop(window, renderer),
    m_window{ window },
    m_renderer{ renderer },
    m_testImgui{ make_unique<TestImgui>() }
{}

bool ToolMainLoop::InitializeDerived()
{
    m_dialog = make_unique<Dialog>();

    return true;
}

bool ToolMainLoop::LoadResources(const wstring& resPath)
{
    m_renderer->AddImguiItem(m_testImgui.get());

    Rectangle rect{ 0, 0, 220, 190 };
    UILayout layout(move(rect), { 0.0f, 0.0f }, Origin::LeftTop);
    ImageSource dialogSource{
        L"UI/Blue/button_square_header_large_square_screws.png", {
            { 0, 0, 30, 36 }, { 30, 0, 4, 36 }, { 34, 0, 30, 36 },
            { 0, 36, 30, 2 }, { 30, 36, 4, 2 }, { 34, 36, 30, 2 },
            { 0, 38, 30, 26 }, { 30, 38, 4, 26 }, { 34, 38, 30, 26 }
        }
    };

    m_dialog->SetImage(resPath, m_renderer, dialogSource, layout);
    m_renderer->AddLoadResource(m_dialog.get());
    m_renderer->AddRenderItem(m_dialog.get());

    m_guiWidget = make_unique<GuiWidget>(m_renderer);
    ReturnIfFalse(m_guiWidget->Create(m_renderer, m_dialog.get()));
    m_renderer->AddImguiItem(m_guiWidget.get());
    
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
    m_guiWidget->Update();

    PIXEndEvent();
}