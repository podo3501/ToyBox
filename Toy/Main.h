#pragma once

class Game;

class MainLoop
{
public:
	MainLoop();
	~MainLoop();
	bool Initialize(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow);
	int Run();
	
private:
	bool CreateGameWindow(HINSTANCE hInstance, RECT& rc, HWND& hwnd);

	std::unique_ptr<Game> m_game;
};