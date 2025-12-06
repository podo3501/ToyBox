#pragma once
#include "Internal/MockRenderer.h"
#include "Internal/MockInputManager.h"
#include "Shared/Framework/EnvironmentLocator.h"
#include "Toy/UserInterface/UIComponentLocator.h"
#include "Toy/Locator/InputLocator.h"

class TextureResourceBinder;
namespace DX { class StepTimer; }
class UIComponentManagerT : public ::testing::Test
{
protected:
	virtual void SetUp() override;
	virtual void RegisterRenderTextures(MockRenderer* renderer);
	inline ITextureController* GetTextureController() const noexcept { return m_renderer->GetTextureController(); }
	inline wstring GetTempDir() const noexcept { return L"../Resources/Test/Temp/"; }
	void BindTextureSourceInfo(UIComponent* component, TextureResourceBinder* resBinder);
	void Update(UIComponent* component, DX::StepTimer& timer);
	void Render(UIComponent* component, ITextureRender* render);

	unique_ptr<Environment> m_environment;
	unique_ptr<MockInputManager> m_input;
	unique_ptr<MockRenderer> m_renderer;
	unique_ptr<UIComponentManager> m_componentManager;
};