#pragma once

#include "MainLoop.h"

class Button;
class Dialog;
class TextArea;

class GameMainLoop final : public MainLoop
{
public:
	GameMainLoop();
	~GameMainLoop();

protected:
	virtual bool InitializeDerived() override;
	virtual bool LoadResources(const wstring& resPath) override;
	virtual bool SetDatas(IGetValue* getValue) override;
	virtual void Update(const DX::StepTimer* timer, const Vector2& resolution, const Mouse::ButtonStateTracker* mouseTracker) override;

private:
	unique_ptr<Button>m_button;
	unique_ptr<Button>m_button2;
	unique_ptr<Button>m_closeButton;
	unique_ptr<Dialog> m_dialog;
	unique_ptr<TextArea> m_textArea;
};