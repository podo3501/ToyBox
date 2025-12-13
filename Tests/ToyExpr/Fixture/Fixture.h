#pragma once
#include "Shared/System/StepTimer.h"

struct IRenderer;
struct IComponent;
struct ITextureController;
struct IJsonStorage;
class Window;
class Environment;
class UIComponentManager;
class UIModule;
class TextureResourceBinder;
class Panel;

class Fixture : public ::testing::Test
{
public:
	Fixture();
	~Fixture();

protected:
	void SetUp() override;
	void TearDown() override;
	TextureResourceBinder* GetResBinder() const noexcept;
	ITextureController* GetTextureController() noexcept;

	unique_ptr<Window> m_window;
	unique_ptr<Environment> m_environment;
	unique_ptr<IJsonStorage> m_jsonStorage;
	unique_ptr<IRenderer> m_renderer;
	DX::StepTimer m_timer;
	unique_ptr<UIComponentManager> m_uiManager;
	UIModule* m_uiModule{ nullptr };
	Panel* m_main{ nullptr };
};