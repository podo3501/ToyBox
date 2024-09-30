#pragma once

struct IRenderItem;

struct IRenderer
{
public:
    virtual ~IRenderer() {};

    virtual bool Initialize() = 0;
    virtual bool LoadResources(IRenderItem* item) = 0;

    virtual void OnActivated() = 0;
    virtual void OnDeactivated() = 0;
    virtual void OnSuspending() = 0;
    virtual void OnResuming() = 0;
    virtual void OnWindowMoved() = 0;
    virtual void OnDisplayChange() = 0;
    virtual void OnWindowSizeChanged(int width, int height) = 0;

    virtual void Draw(IRenderItem* item) = 0;
};

std::unique_ptr<IRenderer> CreateRenderer(HWND hwnd, int width, int height);