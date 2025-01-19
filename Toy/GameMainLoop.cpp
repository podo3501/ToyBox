#include "pch.h"
#include "GameMainLoop.h"
#include "../Include/IRenderer.h"
#include "Utility.h"
#include "Config.h"
#include "UserInterface/Component/Panel.h"
#include "UserInterface/Component/SampleComponent.h"

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

GameMainLoop::~GameMainLoop() = default;
GameMainLoop::GameMainLoop(Window* window, IRenderer* renderer) :
    ::MainLoop(window, renderer),
    m_window{ window },
    m_renderer{ renderer }
{}

bool GameMainLoop::InitializeDerived()
{
    UILayout layout{ RectangleToXMUINT2(GetRectResolution()), Origin::LeftTop };
    m_gamePanel = make_unique<Panel>("Main", layout);
    m_renderer->AddRenderComponent(m_gamePanel.get());

    return true;
}

bool GameMainLoop::LoadResources()
{
    unique_ptr<UIComponent> button = CreateSampleButton3({ { 180, 48 }, Origin::Center });
    unique_ptr<UIComponent> button2 = CreateSampleButton3({ { 180, 48 }, Origin::Center });

    m_gamePanel->AttachComponent(move(button), { 400, 300 });
    m_gamePanel->AttachComponent(move(button2), { 400, 240 });

    unique_ptr<UIComponent> closeButton = CreateSampleButton1({ { 32, 32 }, Origin::Center });
    m_gamePanel->AttachComponent(move(closeButton), { 160, 120 });

    unique_ptr<UIComponent> textArea = CreateSampleTextArea({ { 250, 120 }, Origin::Center });
    m_gamePanel->AttachComponent(move(textArea), { 160, 420 });

    //UILayout layout({ 210, 150 }, Origin::LeftTop);
    //ImageSource img9Source{
    //    L"UI/Blue/button_square_header_large_square_screws.png", {
    //        { 0, 0, 30, 36 }, { 30, 0, 4, 36 }, { 34, 0, 30, 36 },
    //        { 0, 36, 30, 2 }, { 30, 36, 4, 2 }, { 34, 36, 30, 2 },
    //        { 0, 38, 30, 26 }, { 30, 38, 4, 26 }, { 34, 38, 30, 26 }
    //    }
    //};

    //unique_ptr<ImageGrid9> imgGrid9 = make_unique<ImageGrid9>();
    //imgGrid9->SetImage(layout, img9Source);

    unique_ptr<UIComponent> imgGrid9 = CreateSampleImageGrid9({ { 210, 150 }, Origin::LeftTop });
    m_gamePanel->AttachComponent(move(imgGrid9), { 400, 300 });

    m_renderer->LoadComponent(m_gamePanel.get());

    return true;
}

void GameMainLoop::Update(const DX::StepTimer* timer, const InputManager& inputManager)
{
    PIXBeginEvent(PIX_COLOR_DEFAULT, L"Update");

    UNREFERENCED_PARAMETER(timer);
    //float elapsedTime = float(timer->GetElapsedSeconds());

    m_gamePanel->ProcessUpdate({}, inputManager);

    PIXEndEvent();
}