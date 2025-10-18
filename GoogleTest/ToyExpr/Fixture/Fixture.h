#pragma once
#include "Shared/System/StepTimer.h"

class Window;
class UIComponent;
class UIModule;
class TextureResourceBinder;
struct IRenderer;
struct IComponent;

class Fixture : public ::testing::Test
{
public:
	Fixture();
	~Fixture();

protected:
	void SetUp() override;
	void TearDown() override;
	TextureResourceBinder* GetResBinder() const noexcept;

	unique_ptr<Window> m_window;
	unique_ptr<IRenderer> m_renderer;
	DX::StepTimer m_timer;
	unique_ptr<UIModule> m_uiModule;
	UIComponent* m_main;
};