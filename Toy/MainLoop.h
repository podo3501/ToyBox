#pragma once

class Game;
class Window;
class Button;

class MainLoop
{
public:
	MainLoop();
	~MainLoop();
	bool Initialize(HINSTANCE hInstance, const std::wstring& resPath, int nCmdShow);
	LRESULT WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	int Run();
	
private:
	bool InitializeClass(HINSTANCE hInstance, const std::wstring& resPath, int nCmdShow);
	void AddWinProcListener();
	void Update();

	std::unique_ptr<Game> m_game;
	std::unique_ptr<Window> m_window;
	std::unique_ptr<DirectX::Mouse> m_mouse;
	std::unique_ptr<Button> m_button;
};