#pragma once

class Window;
struct IRenderer;
struct IRenderItem;
struct IImguiItem;
struct IGetValue;

namespace DX
{
    class StepTimer;
}

class MainLoop
{
public:
    MainLoop();
    virtual ~MainLoop();
    bool Initialize(HINSTANCE hInstance, const wstring& resPath, int nCmdShow, bool bUseImgui);
    int Run();

protected:
    virtual bool InitializeDerived() = 0;
    virtual bool LoadResources(const wstring& resPath) = 0;
    virtual bool SetDatas(IGetValue* getValue) = 0;
    virtual void Update(const DX::StepTimer* timer, const Vector2& resolution, const Mouse::ButtonStateTracker* mouseTracker) = 0;

    void AddRenderItem(IRenderItem* item) noexcept;
    void AddImguiItem(IImguiItem* item) noexcept;

    unique_ptr<IRenderer> m_renderer;

private:
    LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
    bool InitializeClass(HINSTANCE hInstance, int nCmdShow, bool bUseImgui);
    void AddWinProcListener(bool bUseImgui) noexcept;
    void Tick();
    void OnResuming() const;

    bool m_sizemove{ false };
    bool m_suspend{ false };
    bool m_minimized{ false };
    bool m_fullscreen{ false };

    wstring m_resourcePath;
    unique_ptr<Window> m_window;
    unique_ptr<DirectX::Mouse> m_mouse;
    unique_ptr<DX::StepTimer> m_timer;
};