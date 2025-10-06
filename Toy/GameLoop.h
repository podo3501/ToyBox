#pragma once
#include "Shared/Framework/AppLoop.h"

class UIComponent;
class UIModule;

class GameLoop final : public AppLoop
{
public:
	GameLoop() = delete;
	GameLoop(unique_ptr<Window> window, unique_ptr<IRenderer> renderer);
	virtual ~GameLoop();

protected:
	virtual bool InitializeDerived() override;
	virtual bool LoadResources() override;
	virtual bool PostLoaded(ITextureController*) override { return true; }
	virtual void Update(const DX::StepTimer& timer) override;

private:
	bool AttachComponentToPanel(unique_ptr<UIComponent> component, const XMINT2& position) const noexcept;

	IRenderer* m_renderer;
	unique_ptr<UIModule> m_uiModule;
};