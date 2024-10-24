//
// Game.h
//

#pragma once

#include "../Include/IRenderer.h"
#include "../Include/IRenderItem.h"
#include "IDeviceNotify.h"

struct IImgui;
class TextureIndexing;
class RenderTexture;

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

    Renderer(HWND hwnd, int width, int height, unique_ptr<IImgui>&& imgui) noexcept(false);
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
    virtual void AddImguiItem(IImguiItem* item) override;
    virtual bool LoadResources() override;
    virtual IGetValue* GetValue() const noexcept override;
    virtual bool CreateRenderTexture(const XMUINT2& size, IRenderItem* renderItem, ImTextureID& outTextureID) override;
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
    unique_ptr<DX::DeviceResources> m_deviceResources;
    unique_ptr<GraphicsMemory> m_graphicsMemory;
    unique_ptr<DescriptorHeap> m_srvDescriptors;
    unique_ptr<ResourceUploadBatch> m_batch;

    unique_ptr<IImgui> m_imgui;
    unique_ptr<SpriteBatch> m_spriteBatch;
    unique_ptr<TextureIndexing> m_texIndexing;
    unique_ptr<RenderTexture> m_renderTexture;
    vector<IRenderItem*> m_renderItems;
};
