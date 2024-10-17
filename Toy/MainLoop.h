#pragma once

class Window;
class Button;
class Dialog;
class TextArea;

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
	bool Initialize(HINSTANCE hInstance, const wstring& resPath, int nCmdShow);
	int Run();
	
private:
	LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	bool InitializeClass(HINSTANCE hInstance, int nCmdShow);
	void AddWinProcListener();
	void Tick();
	void OnResuming();
	void Update(DX::StepTimer* timer);

	wstring m_resourcePath;
	unique_ptr<IRenderer> m_renderer;
	unique_ptr<Window> m_window;
	unique_ptr<DirectX::Mouse> m_mouse;
	unique_ptr<Button>m_button;
	unique_ptr<Button>m_button2;
	unique_ptr<Button>m_closeButton;
	unique_ptr<Dialog> m_dialog;
	unique_ptr<TextArea> m_textArea;
	unique_ptr<DX::StepTimer> m_timer;

	bool m_sizemove{ false };
	bool m_suspend{ false };
	bool m_minimized{ false };
	bool m_fullscreen{ false };
};