#include "pch.h"
#include "GameMainLoop.h"
#include "../Include/IRenderer.h"
#include "Utils/GeometryUtil.h"
#include "GameConfig.h"
#include "UserInterface/UIComponent/Components/ListArea.h"
#include "UserInterface/UIComponent/Components/Button.h"
#include "UserInterface/UIComponent/Components/TextArea.h"
#include "UserInterface/UIComponent/Components/PatchTexture/PatchTextureStd/PatchTextureStd9.h"
#include "UserInterface/UIComponent/Components/SampleComponent.h"
#include "UserInterface/UIComponent/Components/TextureSwitcher.h"
#include "UserInterface/TextureResourceBinder/TextureResourceBinder.h"
#include "UserInterface/UIModule.h"

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
    m_uiModule = CreateUIModule(layout, "Main", m_renderer, L"UI/SampleTexture/SampleTextureBinder.json");
    m_uiModule->AddRenderer();
    m_renderer->LoadTextureBinder(m_uiModule->GetTexResBinder());

    return true;
}

bool GameMainLoop::AttachComponentToPanel(unique_ptr<UIComponent> component, const XMINT2& position) const noexcept
{
    if (!component) return false;

    UIComponent* main = m_uiModule->GetComponent();
    UIEx(main).AttachComponent(move(component), position);

    return true;
}

bool GameMainLoop::LoadResources()
{
    AttachComponentToPanel(CreateComponent<TextureSwitcher>(UILayout{ {32, 32}, Origin::Center }, TextureSlice::One, GetStateKeyMap("ExitButton1"), BehaviorMode::Normal), { 100, 100 });
    AttachComponentToPanel(CreateComponent<TextureSwitcher>(UILayout{ {180, 48}, Origin::Center }, TextureSlice::ThreeH, GetStateKeyMap("ScrollButton3_H"), BehaviorMode::Normal), { 400, 300 });
    AttachComponentToPanel(CreateComponent<TextureSwitcher>(UILayout{ {180, 48}, Origin::Center }, TextureSlice::ThreeH, GetStateKeyMap("ScrollButton3_H"), BehaviorMode::Normal), { 400, 240 });
    vector<wstring> bindFontKeys{ L"English", L"Hangle" };
    AttachComponentToPanel(CreateComponent<TextArea>(UILayout{ {250, 120}, Origin::Center }, L"<Hangle>테스트 입니다!</Hangle> <English><Red>Test!</Red></English>", bindFontKeys), { 160, 420 });
    AttachComponentToPanel(CreateComponent<PatchTextureStd9>(UILayout{ {210, 150}, Origin::LeftTop }, "BackImage9"), { 400, 300 });
    AttachComponentToPanel(CreateSampleListArea({ {200, 170}, Origin::Center }), { 600, 200 });
    ReturnIfFalse(m_uiModule->BindTextureResources());

    ListArea* list = UIEx(m_uiModule->GetComponent()).FindComponent<ListArea*>("ListArea");
    MakeSampleListAreaData(m_renderer, m_uiModule->GetTexResBinder(), list, 13);
    
    return true;
}

void GameMainLoop::Update(const DX::StepTimer& timer)
{
    PIXBeginEvent(PIX_COLOR_DEFAULT, L"Update");

    m_uiModule->Update(timer);

    PIXEndEvent();
}