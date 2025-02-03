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

static inline void AttachComponentToPanel(UIComponent* panel, 
    unique_ptr<UIComponent> component, const XMINT2& position)
{
    if (panel && component)
        UIEx(panel).AttachComponent(move(component), position);
}

bool GameMainLoop::LoadResources()
{
    AttachComponentToPanel(m_gamePanel.get(), CreateSampleButton3({ {180, 48}, Origin::Center }), { 400, 300 });
    AttachComponentToPanel(m_gamePanel.get(), CreateSampleButton3({ {180, 48}, Origin::Center }), { 400, 240 });
    AttachComponentToPanel(m_gamePanel.get(), CreateSampleButton1({ {32, 32}, Origin::Center }), { 160, 120 });
    AttachComponentToPanel(m_gamePanel.get(), CreateSampleTextArea({ {250, 120}, Origin::Center }, L"<Hangle>테스트 입니다!</Hangle> <English><Red>Test!</Red></English>"), { 160, 420 });
    AttachComponentToPanel(m_gamePanel.get(), CreateSampleImageGrid9({ {210, 150}, Origin::LeftTop }), { 400, 300 });

    m_renderer->LoadComponent(m_gamePanel.get());
    
    return true;
}

void GameMainLoop::Update(const DX::StepTimer* timer)
{
    PIXBeginEvent(PIX_COLOR_DEFAULT, L"Update");

    UNREFERENCED_PARAMETER(timer);
    //float elapsedTime = float(timer->GetElapsedSeconds());

    m_gamePanel->ProcessUpdate({}, true);

    PIXEndEvent();
}