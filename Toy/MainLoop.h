#pragma once

class Window;
class MouseTracker;
struct IRenderer;
struct IImguiItem;
struct IGetValue;

namespace DX
{
    class StepTimer;
}

class MainLoop
{
public:
    MainLoop() = delete;
    MainLoop(Window* window, IRenderer* renderer);
    virtual ~MainLoop();

    bool Initialize(const wstring& resPath, const Vector2& resolution);
    int Run();

protected:
    virtual bool InitializeDerived() = 0;
    virtual bool LoadResources() = 0;
    virtual bool SetDatas(IGetValue* getValue) = 0;
    virtual void Update(const DX::StepTimer* timer, MouseTracker* mouseTracker) = 0;

private:
    LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
    bool InitializeClass();
    void AddWinProcListener() noexcept;
    void Tick();
    void OnResuming() const;

    bool m_sizemove{ false };
    bool m_suspend{ false };
    bool m_minimized{ false };
    bool m_fullscreen{ false };

    wstring m_resourcePath;
    Window* m_window;
    IRenderer* m_renderer;
    unique_ptr<DirectX::Mouse> m_mouse;
    unique_ptr<DX::StepTimer> m_timer;
};