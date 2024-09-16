#pragma once

class Game;

class Window
{
    using WndProcListener = std::function<LRESULT(HWND, UINT, WPARAM, LPARAM)>;

public:
    Window();
    ~Window();
    bool Create(HINSTANCE hInstance, int nCmdShow, RECT& rc, HWND& hwnd);
    inline void AddWndProcListener(WndProcListener listener);
    void GetWindowSize(int& width, int& height) const noexcept;
    
private:
    bool WindowRegisterClass(HINSTANCE hInstance, const std::wstring& className);
    bool CreateGameWindow(HINSTANCE hInstance, RECT& rc, HWND& hwnd);
    LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
    LRESULT CALLBACK MsgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

    HWND m_wnd{};
    int m_width{ 800 };
    int m_height{ 600 };
    HINSTANCE m_hInstance{};
    std::wstring m_className{ L"ToyWindowClass" };
    std::vector<WndProcListener> m_wndProcListeners{};
};

inline void Window::AddWndProcListener(WndProcListener listener) { m_wndProcListeners.emplace_back(std::move(listener)); }