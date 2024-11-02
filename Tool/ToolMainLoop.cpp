#include "pch.h"
#include "ToolMainLoop.h"
#include "TestImgui.h"
#include "GuiWidget.h"
#include "GuiAppWindow.h"
#include "../Toy/Utility.h"
#include "../Toy/UserInterface/UIType.h"
#include "../Toy/UserInterface/BGImage.h"
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
    m_testImgui{ make_unique<TestImgui>(renderer) }
{}

bool ToolMainLoop::InitializeDerived()
{
    m_bgImage = make_unique<BGImage>();

    return true;
}

bool ToolMainLoop::LoadResources(const wstring& resPath)
{
    Rectangle rect{ 0, 0, 220, 190 };
    UILayout layout(move(rect), { 0.0f, 0.0f }, Origin::LeftTop);
    ImageSource dialogSource{
        L"Resources/UI/Blue/button_square_header_large_square_screws.png", {
            { 0, 0, 30, 36 }, { 30, 0, 4, 36 }, { 34, 0, 30, 36 },
            { 0, 36, 30, 2 }, { 30, 36, 4, 2 }, { 34, 36, 30, 2 },
            { 0, 38, 30, 26 }, { 30, 38, 4, 26 }, { 34, 38, 30, 26 }
        }
    };

    m_bgImage->SetImage(m_renderer, dialogSource, layout);
    m_renderer->AddRenderItem(m_bgImage.get());

    m_guiAppWindow = make_unique<GuiAppWindow>(m_renderer);
    ReturnIfFalse(m_guiAppWindow->Create(m_bgImage.get(), { 400, 300 }));
    
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

    m_bgImage->Update(resolution);

    PIXEndEvent();
}