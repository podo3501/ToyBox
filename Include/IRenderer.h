#pragma once

struct IRenderItem;

struct ILoadData
{
public:
    virtual ~ILoadData() {};

    virtual bool LoadTexture(size_t index, const std::wstring& filename, DirectX::XMUINT2* outSize) = 0;
};

struct IRender
{
public:
    virtual ~IRender() {};

    virtual void Render(size_t index, const DirectX::XMUINT2& size, const DirectX::SimpleMath::Vector2& position, const DirectX::XMFLOAT2& origin) = 0;
};

struct IRenderer
{
public:
    virtual ~IRenderer() {};

    virtual bool Initialize() = 0;
    virtual void AddRenderItem(IRenderItem* item) = 0;
    virtual bool LoadResources() = 0;

    virtual void OnActivated() = 0;
    virtual void OnDeactivated() = 0;
    virtual void OnSuspending() = 0;
    virtual void OnResuming() = 0;
    virtual void OnWindowMoved() = 0;
    virtual void OnDisplayChange() = 0;
    virtual void OnWindowSizeChanged(int width, int height) = 0;

    virtual void Draw() = 0;
};

std::unique_ptr<IRenderer> CreateRenderer(HWND hwnd, int width, int height);