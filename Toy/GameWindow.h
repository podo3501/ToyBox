#pragma once

class Game;

class GameWindow
{
public:
    bool Create(HINSTANCE hInstance, int nCmdShow, Game* game, RECT& rc, HWND& hwnd);
    
private:
    bool CreateGameWindow(HINSTANCE hInstance, Game* game, RECT& rc, HWND& hwnd);


};