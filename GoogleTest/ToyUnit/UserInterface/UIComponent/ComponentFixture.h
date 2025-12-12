#pragma once
#include "Internal/MockRenderer.h"
#include "Internal/MockInputManager.h"
#include "UserInterface/TextureResourceBinder/MockTextureResourceBinder.h"
#include "UserInterface/UIComponent/ComponentHelper.h"
#include "Shared/Framework/EnvironmentLocator.h"
#include "Shared/Utils/GeometryExt.h"
#include "Toy/Locator/EventDispatcherLocator.h"
#include "Toy/UserInterface/UIComponent/Traverser/UITraverser.h"
#include "Toy/UserInterface/UIComponent/Traverser/UIDetailTraverser.h"

using namespace UITraverser;
using namespace UIDetailTraverser;

class ComponentFixture : public ::testing::Test
{
protected:
	virtual void SetUp() override;
	virtual void RegisterRenderTextures(MockRenderer* renderer);
	virtual void RegisterBinderTextures(MockTextureResourceBinder* resBinder) = 0;

	inline MockTextureResourceBinder* GetResBinder() const noexcept { return m_resBinder.get(); }
	inline MockRenderer* GetRenderer() const noexcept { return m_renderer.get(); }
	inline wstring GetTempDir() const noexcept { return L"../Resources/Test/Temp/"; }

private:
	unique_ptr<Environment> m_environment;
	unique_ptr<MockInputManager> m_input;
	unique_ptr<MockRenderer> m_renderer;
	unique_ptr<MockTextureResourceBinder> m_resBinder;
	unique_ptr<IEventDispatcherManager> m_nullEventDispatcher;
};