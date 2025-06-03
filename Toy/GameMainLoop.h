#pragma once
#include "MainLoop.h"

class UIComponent;
class UIModule;

class GameMainLoop final : public MainLoop
{
public:
	GameMainLoop() = delete;
	GameMainLoop(Window* window, IRenderer* renderer);
	virtual ~GameMainLoop();

protected:
	virtual bool InitializeDerived() override;
	virtual bool LoadResources() override;
	virtual bool PostLoaded(ITextureController*) override { return true; }
	virtual void Update(const DX::StepTimer& timer) override;

private:
	bool AttachComponentToPanel(unique_ptr<UIComponent> component, const XMINT2& position) const noexcept;

	Window* m_window;
	IRenderer* m_renderer;
	unique_ptr<UIModule> m_uiModule;
};