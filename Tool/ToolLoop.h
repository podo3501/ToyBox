#pragma once
#include "Shared/Framework/AppLoop.h"

struct IRenderer;
class Window;
class TestImgui;

class ToolSystem;
class GuiWidget;

class ToolLoop final : public AppLoop
{
public:
	ToolLoop() = delete;
	ToolLoop(Window* window, IRenderer* renderer);
	virtual ~ToolLoop();

protected:
	virtual bool InitializeDerived() override;
	virtual bool LoadResources() override;
	virtual bool PostLoaded(ITextureController*) override;
	virtual void Update(const DX::StepTimer& timer) override;

private:
	Window* m_window;
	IRenderer* m_renderer;

	unique_ptr<ToolSystem> m_toolSystem;
};

