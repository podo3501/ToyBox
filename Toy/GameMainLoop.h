#pragma once
#include "MainLoop.h"

class Button;
class TextArea;
class UIComponent;

class GameMainLoop final : public MainLoop
{
public:
	GameMainLoop() = delete;
	GameMainLoop(Window* window, IRenderer* renderer);
	virtual ~GameMainLoop();

protected:
	virtual bool InitializeDerived() override;
	virtual bool LoadResources() override;
	virtual bool SetDatas(IGetValue*) override { return true; }
	virtual void Update(const DX::StepTimer* timer, MouseTracker* mouseTracker) override;

private:
	Window* m_window;
	IRenderer* m_renderer;

	unique_ptr<UIComponent> m_gamePanel;
};