#pragma once
#include "UserInterface/TextureResourceBinder/MockTextureResourceBinder.h"
#include "UserInterface/UIComponentManagerT.h"
#include "Internal/MockRenderer.h"
#include "Shared/Framework/EnvironmentLocator.h"
#include "Toy/Locator/EventDispatcherLocator.h"
#include "Toy/UserInterface/UIComponent/Traverser/BaseTraverser.h"
#include "Toy/UserInterface/UIComponent/Traverser/DerivedTraverser.h"
#include "Toy/UserInterface/UIComponent/Traverser/NameTraverser.h"

class ComponentT : public UIComponentManagerT
{
protected:
	virtual void SetUp() override;
	virtual void RegisterBinderTextures(MockTextureResourceBinder* resBinder) = 0;

	inline MockTextureResourceBinder* GetResBinder() const noexcept { return m_resBinder.get(); }
	inline wstring GetTempDir() const noexcept { return L"../Resources/Test/Temp/"; }

	BaseTraverser m_baseTraverser;
	DerivedTraverser m_derivedTraverser;
	NameTraverser m_nameTraverser;

private:
	unique_ptr<MockTextureResourceBinder> m_resBinder;
	unique_ptr<IEventDispatcherManager> m_nullEventDispatcher;
};