//
// Game.h
//

#pragma once

#include "DeviceResources.h"

struct RenderItem;
class Texture;
class Button;

namespace DX
{
    class StepTimer;
}

// A basic game implementation that creates a D3D12 device and
// provides a game loop.
class Renderer final : public DX::IDeviceNotify
{
    using DeviceLostListener = std::function<void()>;

public:

    Renderer() noexcept(false);
    ~Renderer();

    Renderer(Renderer&&) = default;
    Renderer& operator= (Renderer&&) = default;

    Renderer(Renderer const&) = delete;
    Renderer& operator= (Renderer const&) = delete;

    // Initialization and management
    bool Initialize(HWND window, int width, int height);

    // IDeviceNotify
    void OnDeviceLost() override;
    void OnDeviceRestored() override;

    // Messages
    void OnActivated();
    void OnDeactivated();
    void OnSuspending();
    void OnResuming();
    void OnWindowMoved();
    void OnDisplayChange();
    void OnWindowSizeChanged(int width, int height);

    void SetRenderItem(RenderItem* item);
    void Draw(DX::StepTimer* timer);

private:
    void Clear();

    void CreateDeviceDependentResources();
    void CreateWindowSizeDependentResources();

    // Device resources.
    std::unique_ptr<DX::DeviceResources> m_deviceResources;

    // If using the DirectX Tool Kit for DX12, uncomment this line:
    std::unique_ptr<DirectX::GraphicsMemory> m_graphicsMemory;

    std::vector<RenderItem*> m_renderItems;

    std::unique_ptr<DirectX::DescriptorHeap> m_resourceDescriptors;

    std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;
};
