//
// Game.h
//

#pragma once

#include "../Include/IRenderer.h"
#include "IDeviceNotify.h"

struct IImgui;
struct IComponent;
class TextureIndexing;
class RenderTexture;
class CycleIterator;

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

    virtual void AddRenderComponent(IComponent* component) override;
    virtual void RemoveRenderComponent(IComponent* component) override;

    virtual void AddImguiComponent(IImguiComponent* item) override;
    virtual void RemoveImguiComponent(IImguiComponent* comp) noexcept override;

    virtual bool LoadComponent(IComponent* component) override;
    //virtual bool LoadComponents() override;

    virtual IGetValue* GetValue() const noexcept override;
    virtual bool CreateRenderTexture(const XMUINT2& size, IComponent* component, ImTextureID& outTextureID) override;
    virtual void RemoveRenderTexture(ImTextureID textureID) override;
    virtual bool ModifyRenderTexture(ImTextureID id, const XMUINT2& size) override;
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
    //unique_ptr<DescriptorHeap> m_srvDescriptors;
    unique_ptr<DescriptorPile> m_srvDescriptorPile;
    unique_ptr<ResourceUploadBatch> m_batch;

    unique_ptr<IImgui> m_imgui;
    unique_ptr<SpriteBatch> m_spriteBatch;
    unique_ptr<TextureIndexing> m_texIndexing;

    unique_ptr<CycleIterator> m_renderTexOffset;
    map<ImTextureID, unique_ptr<RenderTexture>> m_renderTextures;

    vector<IComponent*> m_components;
};
