#pragma once
#include <windows.h>
#include "IRenderItem.h"

struct IRenderer
{
public:
    virtual ~IRenderer() {};

    virtual bool LoadResources() = 0;

    virtual void OnActivated() = 0;
    virtual void OnDeactivated() = 0;
    virtual void OnSuspending() = 0;
    virtual void OnResuming() = 0;
    virtual void OnWindowMoved() = 0;
    virtual void OnDisplayChange() = 0;
    virtual void OnWindowSizeChanged(int width, int height) = 0;

    virtual void SetRenderItem(IRenderItem* item) = 0;
    virtual void Draw() = 0;
};

std::unique_ptr<IRenderer> CreateRenderer(HWND hwnd, int width, int height);