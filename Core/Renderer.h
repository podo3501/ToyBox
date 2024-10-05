//
// Game.h
//

#pragma once

#include "../Include/IRenderer.h"
#include "../Include/IRenderItem.h"
#include "IDeviceNotify.h"

class Texture;
class Button;

namespace DX
{
    class DeviceResources;
}

namespace DirectX
{
    class ResourceUploadBatch;
}

// A basic game implementation that creates a D3D12 device and
// provides a game loop.
class Renderer final : public DX::IDeviceNotify, public IRenderer, public ILoadData, public IRender
{
    using DeviceLostListener = function<void()>;

public:

    Renderer(HWND hwnd, int width, int height) noexcept(false);
    ~Renderer();

    Renderer(Renderer&&) = default;
    Renderer& operator= (Renderer&&) = default;

    Renderer(Renderer const&) = delete;
    Renderer& operator= (Renderer const&) = delete;

    // IDeviceNotify
    void OnDeviceLost() override;
    void OnDeviceRestored() override;

    // Initialization and management
    virtual bool Initialize() override;
    virtual void AddRenderItem(IRenderItem* item) override;
    virtual bool LoadResources() override;

    //ILoadData
    virtual bool LoadTexture(size_t index, const wstring& filename, XMUINT2* outSize) override;
    
    //IRender
    virtual void Render(size_t index, const XMUINT2& size, const Vector2& position, const XMFLOAT2& origin) override;

    // Messages
    virtual void OnActivated() override;
    virtual void OnDeactivated() override;
    virtual void OnSuspending() override;
    virtual void OnResuming() override;
    virtual void OnWindowMoved() override;
    virtual void OnDisplayChange() override;
    virtual void OnWindowSizeChanged(int width, int height) override;

    virtual void Draw() override;

private:
    void Clear();

    void CreateDeviceDependentResources();
    void CreateWindowSizeDependentResources();

    // Device resources.
    std::unique_ptr<DX::DeviceResources> m_deviceResources;
    std::unique_ptr<GraphicsMemory> m_graphicsMemory;
    std::unique_ptr<DescriptorHeap> m_resourceDescriptors;
    std::unique_ptr<SpriteBatch> m_spriteBatch;

    std::vector<IRenderItem*> m_renderItems;
    std::map<size_t, unique_ptr<Texture>> m_textures;

    std::unique_ptr<ResourceUploadBatch> m_batch;
};
