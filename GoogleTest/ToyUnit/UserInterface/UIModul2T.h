#pragma once
#include "UIComponentManagerT.h"
#include "Shared/Framework/EnvironmentLocator.h"
#include "Shared/Utils/GeometryExt.h"
#include "Toy/UserInterface/UIComponent/UILayout.h"
#include "Toy/UserInterface/UIComponent/UIType.h"
#include "Toy/UserInterface/TextureResourceBinder/TextureResourceBinder.h"
#include "Toy/UserInterface/UIModul2.h"
#include "Toy/UserInterface/UIComponent/Components/Panel.h"

class UIModul2T : public UIComponentManagerT
{
protected:
	virtual void SetUp() override;
	virtual void TearDown() override {};
	virtual void RegisterBinderTextures(TextureResourceBinder* resBinder) {};
	inline wstring GetTempDir() const noexcept { return L"../Resources/Test/Temp/"; }

	unique_ptr<Environment> m_environment;
	UIModul2* m_uiModule{ nullptr };
	Panel* m_main{ nullptr };
};

inline void UIModul2T::SetUp()
{
	UIComponentManagerT::SetUp();

	m_environment = InitializeEnvironment(L"", { 800.f, 600.f });

	UILayout layout{ GetSizeFromRectangle(GetRectResolution()), Origin::LeftTop };
	m_uiModule = m_componentManager->CreateUIModule(layout, "Main", m_renderer.get());

	RegisterBinderTextures(m_uiModule->GetTexResBinder());
	m_main = m_uiModule->GetMainPanel();
}