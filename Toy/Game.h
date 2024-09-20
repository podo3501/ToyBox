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
class Game final : public DX::IDeviceNotify
{
    using DeviceLostListener = std::function<void()>;

public:

    Game() noexcept(false);
    ~Game();

    Game(Game&&) = default;
    Game& operator= (Game&&) = default;

    Game(Game const&) = delete;
    Game& operator= (Game const&) = delete;

    // Initialization and management
    bool Initialize(HWND window, int width, int height);

    LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

    // Basic game loop
    void Tick();

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

private:
    void Update(DX::StepTimer* timer);
    void Render();

    void Clear();

    void CreateDeviceDependentResources();
    void CreateWindowSizeDependentResources();

    // Device resources.
    std::unique_ptr<DX::DeviceResources> m_deviceResources;

    // Rendering loop timer.
    std::unique_ptr<DX::StepTimer> m_timer;

    // If using the DirectX Tool Kit for DX12, uncomment this line:
    std::unique_ptr<DirectX::GraphicsMemory> m_graphicsMemory;

    std::vector<RenderItem*> m_renderItems;

    std::unique_ptr<DirectX::DescriptorHeap> m_resourceDescriptors;

    std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;

    bool m_on{ false };
    
    bool m_sizemove{ false };
    bool m_suspend{ false };
    bool m_minimized{ false };
    bool m_fullscreen{ false };
};
