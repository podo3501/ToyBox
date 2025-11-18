#include "pch.h"
#include "UIComponentManagerT.h"

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