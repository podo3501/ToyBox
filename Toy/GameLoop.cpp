#include "pch.h"
#include "GameLoop.h"
#include "IRenderer.h"
#include "Shared/Window/Window.h"
#include "Shared/Utils/GeometryExt.h"
#include "Shared/Framework/Locator.h"
#include "Shared/Framework/Environment.h"
#include "System/SceneManager.h"
#include "UserInterface/UIModule.h"
#include "UserInterface/UIComponent/UILayout.h"
#include "UserInterface/TextureResourceBinder/TextureResourceBinder.h"
#include "Scenes/TestScene.h"

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

GameLoop::~GameLoop() = default;
GameLoop::GameLoop(unique_ptr<Window> window, unique_ptr<IRenderer> renderer) :
    ::AppLoop(move(window), move(renderer)),
    m_renderer{ AppLoop::GetRenderer() }
{}

bool GameLoop::InitializeDerived()
{
    m_sceneManager = CreateSceneManager();
    Locator<ISceneManager>::Provide(m_sceneManager.get());

    UILayout layout{ GetSizeFromRectangle(GetRectResolution()) };
    m_uiModule = CreateUIModule(layout, "Main", m_renderer, L"UI/SampleTexture/SampleTextureBinder.json");
    m_uiModule->AddRenderer();
    m_renderer->LoadTextureBinder(m_uiModule->GetTexResBinder());

    m_sceneManager->CreateScene(make_unique<TestScene>("Test", m_uiModule.get()));

    return true;
}

bool GameLoop::DoPrepare()
{
    m_sceneManager->Transition("Test");

    return true;
}

void GameLoop::Update(const DX::StepTimer& timer)
{
    PIXBeginEvent(PIX_COLOR_DEFAULT, L"Update");

    ISceneManager* sceneManager = Locator<ISceneManager>::GetService();
    sceneManager->Update(timer);

    PIXEndEvent();
}