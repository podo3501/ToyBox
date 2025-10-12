#include "pch.h"
#include "GameLoop.h"
#include "IRenderer.h"
#include "Shared/Window/Window.h"
#include "Shared/Framework/Locator.h"
#include "System/SceneManager.h"
#include "Scenes/Test/ComponentTestScene.h"
#include "Scenes/Test/TestScene1.h"

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

    return true;
}

bool GameLoop::DoPrepare()
{
    //m_sceneManager->Transition(make_unique<ComponentTestScene>(m_renderer));
    m_sceneManager->Transition(make_unique<TestScene1>(m_renderer));

    return true;
}

void GameLoop::Update(const DX::StepTimer& timer)
{
    PIXBeginEvent(PIX_COLOR_DEFAULT, L"Update");

    ISceneManager* sceneManager = Locator<ISceneManager>::GetService();
    sceneManager->Update(timer);

    PIXEndEvent();
}