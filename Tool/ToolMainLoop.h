#pragma once
#include "../Toy/MainLoop.h"

struct IRenderer;
class Window;
class TestImgui;

class ToolSystem;
class GuiWidget;

class ToolMainLoop final : public MainLoop
{
public:
	ToolMainLoop() = delete;
	ToolMainLoop(Window* window, IRenderer* renderer);
	virtual ~ToolMainLoop();

protected:
	virtual bool InitializeDerived() override;
	virtual bool LoadResources() override;
	virtual bool PostLoaded(ITextureController*) override;
	virtual void Update(const DX::StepTimer& timer) override;

private:
	Window* m_window;
	IRenderer* m_renderer;

	unique_ptr<TestImgui> m_testImgui;

	unique_ptr<ToolSystem> m_toolSystem;
};

