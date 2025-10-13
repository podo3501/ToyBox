#include "pch.h"
#include "TestScene2.h"
#include "IRenderer.h"
#include "UserInterface/UIModule.h"
#include "System/EventDispatcher.h"
#include "Shared/Framework/Locator.h"
#include "System/SceneManager.h"
#include "TestScene1.h"

TestScene2::TestScene2(IRenderer* renderer) :
	Scene(renderer)
{}

bool TestScene2::Enter()
{
	m_uiModule = CreateUIModule(L"/Scene/Test/TestScene2.json",
		GetRenderer(), L"UI/SampleTexture/SampleTextureBinder.json");
	m_uiModule->AddRenderer();

	ISceneManager* sceneManager = Locator<ISceneManager>::GetService();
	EventDispatcher::Subscribe("", "TextureSwitcher", [this, sceneManager](UIEvent event) {
		if (event == UIEvent::Clicked)
			sceneManager->Transition(make_unique<TestScene1>(GetRenderer()));
		});

	return true;
}

bool TestScene2::Leave()
{
	EventDispatcher::Clear();
	return true;
}

void TestScene2::Update(const DX::StepTimer& timer)
{
	m_uiModule->Update(timer);
}