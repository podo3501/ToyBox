#include "pch.h"
#include "UIComponentManagerT.h"
#include "Toy/UserInterface/Traversers.h"
#include "Toy/UserInterface/UIComponent/Traverser/DerivedTraverser.h"

void UIComponentManagerT::SetUp()
{
	m_environment = InitializeEnvironment(L"", { 800.f, 600.f });

	m_input = make_unique<MockInputManager>();
	InputLocator::Provide(m_input.get());

	m_renderer = make_unique<MockRenderer>();
	RegisterRenderTextures(m_renderer.get());
	
	m_componentManager = make_unique<UIComponentManager>(m_renderer.get());
	UIComponentLocator::Provide(m_componentManager.get());
}

void UIComponentManagerT::RegisterRenderTextures(MockRenderer* renderer)
{
	renderer->RegisterMockTextureInfos({
		{ L"Texture512.png", 0, {512, 512} }
		});
}

void UIComponentManagerT::BindTextureSourceInfo(UIComponent* component, TextureResourceBinder* resBinder)
{
	auto derivedTraverser = m_componentManager->GetTraversers()->GetDerivedTraverser();
	derivedTraverser->BindTextureSourceInfo(component, resBinder);
}

void UIComponentManagerT::Update(UIComponent* component, DX::StepTimer& timer)
{
	auto derivedTraverser = m_componentManager->GetTraversers()->GetDerivedTraverser();
	derivedTraverser->Update(component, timer);
}

void UIComponentManagerT::Render(UIComponent* component, ITextureRender* render)
{
	auto derivedTraverser = m_componentManager->GetTraversers()->GetDerivedTraverser();
	derivedTraverser->Render(component, render);
}