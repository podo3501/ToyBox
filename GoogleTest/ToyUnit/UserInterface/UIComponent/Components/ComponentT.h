#pragma once
#include "UserInterface/TextureResourceBinder/MockTextureResourceBinder.h"
#include "Mocks/MockRenderer.h"
#include "Shared/Framework/EnvironmentLocator.h"
#include "Toy/Locator/EventDispatcherLocator.h"

class ComponentT : public ::testing::Test
{
protected:
	virtual void SetUp() override;
	virtual void TearDown() override {};
	virtual void RegisterRenderTextures(MockRenderer* renderer);
	virtual void RegisterBinderTextures(MockTextureResourceBinder* resBinder) = 0;

	inline ITextureController* GetTextureController() const noexcept { return m_renderer->GetTextureController();	}
	inline MockTextureResourceBinder* GetResBinder() const noexcept { return m_resBinder.get(); }
	inline wstring GetTempDir() const noexcept { return L"../Resources/Test/Temp/"; }

private:
	unique_ptr<Environment> m_environment;
	unique_ptr<MockRenderer> m_renderer;
	unique_ptr<MockTextureResourceBinder> m_resBinder;
	unique_ptr<IEventDispatcherManager> m_nullEventDispatcher;
};

//unique_ptr<IEventDispatcherManager> m_eventDispatcher;
//testing::MockFunction<void(UIEvent)> m_mockCallback;
//};
//
//void EventDispatcherManagerT::SetUp()
//{
//	m_eventDispatcher = CreateEventDispatcherManager();
//	EventDispatcherLocator::Provide(m_eventDispatcher.get());