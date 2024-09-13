#pragma once

class Game;
class GameWindow;

class MainLoop
{
public:
	MainLoop();
	~MainLoop();
	bool Initialize(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow);
	int Run();
	
private:
	std::unique_ptr<Game> m_game;
	std::unique_ptr<GameWindow> m_window;
};