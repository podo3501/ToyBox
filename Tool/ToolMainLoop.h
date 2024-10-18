#pragma once
#include "../Toy/MainLoop.h"

struct ImGuiIO;

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
	virtual void Render() override;
	virtual void CleanUp() override;

private:
	LRESULT WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	ImGuiIO* m_io;
};

