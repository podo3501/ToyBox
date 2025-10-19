#include "pch.h"
#include "TestScene1.h"
#include "IRenderer.h"
#include "Locator/SceneLocator.h"
#include "Locator/EventDispatcherLocator.h"
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

	//���� ���۵ɶ� ����ϰ� ���� ������ �����Ѵ�.
	//���⼭ �ϸ� �� �̸����� �־���� �Ѵ�.
	auto scene = SceneLocator::GetService();
	auto eventDispatcher = EventDispatcherLocator::GetService();
	eventDispatcher->Subscribe("", "TextureSwitcher", [this, scene](UIEvent event) {
		if (event == UIEvent::Clicked)
			scene->Transition(make_unique<TestScene2>(GetRenderer()));
		});

	return true;
}

bool TestScene1::Leave()
{
	auto eventDispatcher = EventDispatcherLocator::GetService();
	eventDispatcher->Clear();

	return true;
}

void TestScene1::Update(const DX::StepTimer& timer)
{
	m_uiModule->Update(timer);
	m_uiModule->UpdateMouseState();
}