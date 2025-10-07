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
    AppLoop(unique_ptr<Window> window, unique_ptr<IRenderer> renderer);
    virtual ~AppLoop();

    bool Initialize(const wstring& resPath, const Vector2& resolution);
    int Run();

protected:
    virtual bool InitializeDerived() { return true; }
    virtual bool LoadResources() { return true; }
    virtual bool PostLoaded(ITextureController*) { return true; }
    virtual void Update(const DX::StepTimer& timer) = 0;
    IRenderer* GetRenderer() const noexcept;

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
    
    unique_ptr<Window> m_window;
    unique_ptr<IRenderer> m_renderer;
    wstring m_resourcePath;
    DX::StepTimer m_timer;
};

template<typename LoopType>
unique_ptr<AppLoop> CreateAppLoop(unique_ptr<Window> window, unique_ptr<IRenderer> renderer,
    const Vector2& windowSize, const wstring& resourcePath)
{
    auto loop = make_unique<LoopType>(move(window), move(renderer));
    if (!loop->Initialize(resourcePath, windowSize))
        return nullptr;

    return loop;
}