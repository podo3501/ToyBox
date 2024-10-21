#pragma once
#include "../Toy/MainLoop.h"

class TestImgui;
class Dialog;

class ToolMainLoop final : public MainLoop
{
public:
	ToolMainLoop();
	virtual ~ToolMainLoop();

protected:
	virtual bool InitializeDerived() override;
	virtual bool LoadResources(const wstring& resPath) override;
	virtual bool SetDatas(IGetValue* getValue) override;
	virtual void Update(const DX::StepTimer* timer, const Vector2& resolution, const Mouse::ButtonStateTracker* mouseTracker) override;

private:
	unique_ptr<TestImgui> m_testImgui;
	unique_ptr<Dialog> m_dialog;
};

