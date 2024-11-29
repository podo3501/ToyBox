#pragma once
#include "MainLoop.h"

struct IRenderScene;
class Button;
class TextArea;
class Panel;

class GameMainLoop final : public MainLoop
{
public:
	GameMainLoop() = delete;
	GameMainLoop(Window* window, IRenderer* renderer);
	virtual ~GameMainLoop();

protected:
	virtual bool InitializeDerived() override;
	virtual bool LoadResources() override;
	virtual bool SetDatas(IGetValue* getValue) override;
	virtual void Update(const DX::StepTimer* timer, MouseTracker* mouseTracker) override;

private:
	Window* m_window;
	IRenderer* m_renderer;

	unique_ptr<IRenderScene> m_gameScene;
};