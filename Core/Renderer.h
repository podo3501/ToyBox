//
// Game.h
//

#pragma once

#include "../Include/IRenderer.h"
#include "../Include/IRenderItem.h"
#include "IDeviceNotify.h"

class TextureIndexing;

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
class Renderer final : public DX::IDeviceNotify, public IRenderer
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
    virtual IUpdate* GetUpdate() const noexcept override;
    virtual void Draw() override;

    // Messages
    virtual void OnActivated() override;
    virtual void OnDeactivated() override;
    virtual void OnSuspending() override;
    virtual void OnResuming() override;
    virtual void OnWindowMoved() override;
    virtual void OnDisplayChange() override;
    virtual void OnWindowSizeChanged(int width, int height) override;

private:
    void Clear();

    void CreateDeviceDependentResources();
    void CreateWindowSizeDependentResources();

    // Device resources.
    std::unique_ptr<DX::DeviceResources> m_deviceResources;
    std::unique_ptr<GraphicsMemory> m_graphicsMemory;
    std::unique_ptr<DescriptorHeap> m_resourceDescriptors;
    std::unique_ptr<ResourceUploadBatch> m_batch;
    std::unique_ptr<SpriteBatch> m_spriteBatch;
    std::unique_ptr<TextureIndexing> m_texIndexing;

    std::vector<IRenderItem*> m_renderItems;
};
