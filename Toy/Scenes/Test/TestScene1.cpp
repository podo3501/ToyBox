#include "pch.h"
#include "TestScene1.h"
#include "IRenderer.h"
#include "Shared/Framework/Locator.h"
#include "System/EventDispatcher.h"
#include "System/SceneManager.h"
#include "UserInterface/UIModule.h"
#include "TestScene2.h"

TestScene1::~TestScene1() = default;
TestScene1::TestScene1(IRenderer* renderer) :
	Scene(renderer)
{}

bool TestScene1::Enter()
{
	m_uiModule = CreateUIModule(L"/Scene/Test/TestScene1.json",
		GetRenderer(), L"UI/SampleTexture/SampleTextureBinder.json");
	m_uiModule->AddRenderer();

	//씬이 시작될때 등록하고 씬이 나갈때 해제한다.
	//여기서 하면 씬 이름까지 넣어줘야 한다.
	ISceneManager* sceneManager = Locator<ISceneManager>::GetService();
	EventDispatcher::Subscribe("", "TextureSwitcher", [this, sceneManager](UIEvent event) {
		if (event == UIEvent::Clicked)
			sceneManager->Transition(make_unique<TestScene2>(GetRenderer()));
		});

	return true;
}

bool TestScene1::Leave()
{
	EventDispatcher::Clear();
	return true;
}

void TestScene1::Update(const DX::StepTimer& timer)
{
	m_uiModule->Update(timer);
	m_uiModule->UpdateMouseState();
}