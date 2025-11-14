#include "pch.h"
#include "ComponentT.h"

void ComponentT::SetUp()
{
	UIComponentManagerT::SetUp();

	m_resBinder = make_unique<MockTextureResourceBinder>();
	RegisterBinderTextures(m_resBinder.get());

	m_renderer->LoadTextureBinder(m_resBinder.get());
	
	m_nullEventDispatcher = CreateNullEventDispatcherManager();
	EventDispatcherLocator::Provide(m_nullEventDispatcher.get());
}