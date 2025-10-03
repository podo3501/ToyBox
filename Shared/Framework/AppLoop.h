#pragma once
#include "Shared/System/StepTimer.h"

class Window;
class MouseTracker;
struct IRenderer;
struct IImguiItem;
struct ITextureController;

class AppLoop
{
public:
    AppLoop() = delete;
    AppLoop(Window* window, IRenderer* renderer);
    virtual ~AppLoop();

    bool Initialize(const wstring& resPath, const Vector2& resolution);
    int Run();

protected:
    virtual bool InitializeDerived() = 0;
    virtual bool LoadResources() = 0;
    virtual bool PostLoaded(ITextureController* texController) = 0;
    virtual void Update(const DX::StepTimer& timer) = 0;

private:
    LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
    bool InitializeClass();
    void AddWinProcListener() noexcept;
    void Tick();
    void OnResuming();

    bool m_sizemove{ false };
    bool m_suspend{ false };
    bool m_minimized{ false };
    bool m_fullscreen{ false };
    
    wstring m_resourcePath;
    Window* m_window;
    IRenderer* m_renderer;
    DX::StepTimer m_timer;
};