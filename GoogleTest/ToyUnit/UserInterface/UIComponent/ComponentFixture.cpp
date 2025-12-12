#include "pch.h"
#include "ComponentFixture.h"
#include "Toy/Locator/InputLocator.h"

void ComponentFixture::SetUp()
{
	m_environment = InitializeEnvironment(L"", { 800.f, 600.f });

	m_input = make_unique<MockInputManager>();
	InputLocator::Provide(m_input.get());

	m_renderer = make_unique<MockRenderer>();
	RegisterRenderTextures(m_renderer.get());

	m_resBinder = make_unique<MockTextureResourceBinder>();
	RegisterBinderTextures(m_resBinder.get());

	m_renderer->LoadTextureBinder(m_resBinder.get());
	
	m_nullEventDispatcher = CreateNullEventDispatcherManager();
	EventDispatcherLocator::Provide(m_nullEventDispatcher.get());
}

void ComponentFixture::RegisterRenderTextures(MockRenderer* renderer)
{
	renderer->RegisterMockTextureInfos({
		{ L"Texture512.png", 0, {512, 512} }
		});
}