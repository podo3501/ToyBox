#pragma once

class Renderer;
class Window;
//class Button;
class Button3;

namespace DX
{
	class StepTimer;
}

struct IRenderer;

class MainLoop
{
public:
	MainLoop();
	~MainLoop();
	bool Initialize(HINSTANCE hInstance, const std::wstring& resPath, int nCmdShow);
	int Run();
	
private:
	LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	bool InitializeClass(HINSTANCE hInstance, const std::wstring& resPath, int nCmdShow);
	void AddWinProcListener();
	void Tick();
	void OnResuming();
	void Update(DX::StepTimer* timer);

	std::unique_ptr<IRenderer> m_renderer;
	std::unique_ptr<Window> m_window;
	std::unique_ptr<DirectX::Mouse> m_mouse;
	//std::unique_ptr<Button> m_button;
	std::unique_ptr<Button3>m_button3;
	std::unique_ptr<DX::StepTimer> m_timer;

	bool m_sizemove{ false };
	bool m_suspend{ false };
	bool m_minimized{ false };
	bool m_fullscreen{ false };
};