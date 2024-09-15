#pragma once

class Game;
class Window;

class MainLoop
{
public:
	MainLoop();
	~MainLoop();
	bool Initialize(HINSTANCE hInstance, const std::wstring& resPath, int nCmdShow);
	int Run();
	
private:
	bool InitializeClass(HINSTANCE hInstance, const std::wstring& resPath, int nCmdShow);
	void AddWinProcListener();

	std::unique_ptr<Game> m_game;
	std::unique_ptr<Window> m_window;
};