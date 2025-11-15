#pragma once
#include "Shared/System/StepTimer.h"

class Window;
class Environment;
class UIComponentManager;
class UIModule;
class TextureResourceBinder;
class Panel;
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
	unique_ptr<Environment> m_environment;
	unique_ptr<IRenderer> m_renderer;
	DX::StepTimer m_timer;
	unique_ptr<UIComponentManager> m_uiManager;
	UIModule* m_uiModule{ nullptr };
	Panel* m_main{ nullptr };
};