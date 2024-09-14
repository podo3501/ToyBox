#pragma once

class Game;

class Window
{
public:
    ~Window();
    bool Create(HINSTANCE hInstance, int nCmdShow, const Game* game, RECT& rc, HWND& hwnd);
    
private:
    bool CreateGameWindow(HINSTANCE hInstance, const Game* game, RECT& rc, HWND& hwnd);

    HWND m_wnd{};
    HINSTANCE m_hInstance{};
    std::wstring m_className{ L"ToyWindowClass" };
};