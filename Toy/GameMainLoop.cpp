#include "pch.h"
#include "GameMainLoop.h"
#include "../Include/IRenderer.h"
#include "Utility.h"
#include "Config.h"
#include "UserInterface/UIComponent/Components/Panel.h"
#include "UserInterface/UIComponent/Components/ListArea.h"
#include "UserInterface/UIComponent/Components/Button.h"
#include "UserInterface/UIComponent/Components/TextArea.h"
#include "UserInterface/UIComponent/Components/PatchTexture/PatchTextureStd/PatchTextureStd9.h"
#include "UserInterface/UIComponent/Components/SampleComponent.h"
#include "UserInterface/UIComponent/Components/RenderTexture.h"
#include "UserInterface/UIComponent/Components/TextureSwitcher.h"
#include "UserInterface/TextureResourceBinder/TextureResourceBinder.h"

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
    UILayout layout{ GetSizeFromRectangle(GetRectResolution()) };
    m_gamePanel = CreateRootPanel("Main", layout, m_renderer);

    return true;
}

bool GameMainLoop::AttachComponentToPanel(unique_ptr<UIComponent> component, const XMINT2& position) const noexcept
{
    if (!component) return false;

    UIEx(m_gamePanel).AttachComponent(move(component), position);
    return true;
}

bool GameMainLoop::LoadResources()
{
    m_resBinder = CreateSourceBinder(L"UI/SampleTexture/SampleTextureBinder.json");
    m_renderer->LoadTextureBinder(m_resBinder.get());

    AttachComponentToPanel(CreateTextureSwitcher({ {32, 32}, Origin::Center }, TextureSlice::One, GetStateKeyMap("ExitButton1"), BehaviorMode::Normal), { 100, 100 });
    AttachComponentToPanel(CreateTextureSwitcher({ {180, 48}, Origin::Center }, TextureSlice::ThreeH, GetStateKeyMap("ScrollButton3_H"), BehaviorMode::Normal), { 400, 300 });
    AttachComponentToPanel(CreateTextureSwitcher({ {180, 48}, Origin::Center }, TextureSlice::ThreeH, GetStateKeyMap("ScrollButton3_H"), BehaviorMode::Normal), { 400, 240 });
    vector<wstring> bindFontKeys{ L"English", L"Hangle" };
    AttachComponentToPanel(CreateTextArea({ {250, 120}, Origin::Center }, L"<Hangle>테스트 입니다!</Hangle> <English><Red>Test!</Red></English>", bindFontKeys), { 160, 420 });
    AttachComponentToPanel(CreatePatchTextureStd9({ {210, 150}, Origin::LeftTop }, "BackImage9"), { 400, 300 });
    AttachComponentToPanel(CreateSampleListArea({ {200, 170}, Origin::Center }), { 600, 200 });
    ReturnIfFalse(m_gamePanel->BindTextureSourceInfo(m_resBinder.get(), m_renderer->GetTextureController()));

    MakeSampleListAreaData(m_renderer, m_resBinder.get(), UIEx(m_gamePanel).FindComponent<ListArea*>("ListArea_0"), 13);
    
    return true;
}

void GameMainLoop::Update(const DX::StepTimer& timer)
{
    PIXBeginEvent(PIX_COLOR_DEFAULT, L"Update");

    UNREFERENCED_PARAMETER(timer);
    //float elapsedTime = float(timer->GetElapsedSeconds());

    m_gamePanel->ProcessUpdate(timer);

    PIXEndEvent();
}