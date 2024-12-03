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
	//virtual bool SetDatas(IGetValue*) override { return true; }
	virtual bool SetDatas(IGetValue*) override;
	virtual void Update(const DX::StepTimer* timer, MouseTracker* mouseTracker) override;

private:
	Window* m_window;
	IRenderer* m_renderer;

	unique_ptr<TestImgui> m_testImgui;

	unique_ptr<ToolSystem> m_toolSystem;
};

