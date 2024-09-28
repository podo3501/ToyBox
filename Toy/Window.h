#pragma once

class Game;

class Window
{
    using WndProcListener = std::function<LRESULT(HWND, UINT, WPARAM, LPARAM)>;

public:
    Window();
    ~Window();
    bool Create(HINSTANCE hInstance, int nCmdShow, const RECT& rc, HWND& hwnd);
    inline void AddWndProcListener(WndProcListener listener);
    inline RECT GetOutputSize() const noexcept;
    inline DirectX::SimpleMath::Vector2 GetResolution() const noexcept;
    
private:
    bool WindowRegisterClass(HINSTANCE hInstance, const std::wstring& className);
    bool CreateGameWindow(HINSTANCE hInstance, const RECT& rc, HWND& hwnd);
    LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
    LRESULT CALLBACK MsgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

    HWND m_wnd{};
    RECT m_outputSize{};
    HINSTANCE m_hInstance{};
    std::wstring m_className{ L"ToyWindowClass" };
    std::vector<WndProcListener> m_wndProcListeners{};
};

inline void Window::AddWndProcListener(WndProcListener listener) { m_wndProcListeners.emplace_back(std::move(listener)); }
inline RECT Window::GetOutputSize() const noexcept { return m_outputSize; }
inline DirectX::SimpleMath::Vector2 Window::GetResolution() const noexcept{
    return { static_cast<float>(m_outputSize.right + m_outputSize.left), 
        static_cast<float>(m_outputSize.bottom + m_outputSize.top) }; }
