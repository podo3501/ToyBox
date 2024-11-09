#pragma once
#include "../Toy/MainLoop.h"

struct IRenderer;
class Window;
class TestImgui;
class Scene;

class GuiAppWindow;
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
	virtual bool SetDatas(IGetValue* getValue) override;
	virtual void Update(const DX::StepTimer* timer, const Vector2& resolution, const Mouse::ButtonStateTracker* mouseTracker) override;

private:
	Window* m_window;
	IRenderer* m_renderer;

	unique_ptr<TestImgui> m_testImgui;
	unique_ptr<Scene> m_toolScene;

	unique_ptr<GuiAppWindow> m_guiAppWindow;
};

