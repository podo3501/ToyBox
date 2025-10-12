#include "pch.h"
#include "TestScene1.h"
#include "IRenderer.h"
#include "UserInterface/UIModule.h"

TestScene1::TestScene1(IRenderer* renderer) :
	Scene(renderer)
{}

bool TestScene1::Enter()
{
	m_uiModule = CreateUIModule(L"/Scene/Test/TestScene1.json",
		GetRenderer(), L"UI/SampleTexture/SampleTextureBinder.json");
	m_uiModule->AddRenderer();

	return true;
}

bool TestScene1::Leave()
{
	return true;
}

void TestScene1::Update(const DX::StepTimer& timer)
{
	m_uiModule->Update(timer);
}