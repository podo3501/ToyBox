#include "pch.h"
#include "ComponentT.h"

void ComponentT::SetUp()
{
	m_environment = InitializeEnvironment(L"", { 800.f, 600.f });

	m_renderer = make_unique<MockRenderer>();
	RegisterRenderTextures(m_renderer.get());

	m_resBinder = make_unique<MockTextureResourceBinder>();
	RegisterBinderTextures(m_resBinder.get());

	m_renderer->LoadTextureBinder(m_resBinder.get());
	
	m_nullEventDispatcher = CreateNullEventDispatcherManager();
	EventDispatcherLocator::Provide(m_nullEventDispatcher.get());
}

void ComponentT::RegisterRenderTextures(MockRenderer* renderer)
{
	renderer->RegisterMockTextureInfos({
		{ L"Texture512.png", 0, {512, 512} }
		});
}
