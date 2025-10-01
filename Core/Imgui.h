#pragma once

struct IImguiComponent;
struct ImGuiIO;
class Renderer;
class Window;

struct IImgui
{
    virtual ~IImgui() {};
    virtual bool Initialize(ID3D12Device* device, DescriptorHeap* descriptorHeap,
        DXGI_FORMAT format, size_t srvOffset) = 0;
    virtual void AddComponent(IImguiComponent* comp) = 0;
    virtual void RemoveComponent(IImguiComponent* comp) noexcept = 0;
    virtual void Render(ID3D12GraphicsCommandList* commandList) = 0;
    virtual void PrepareRender() = 0;
    virtual void Reset() = 0;
};

//imgui�� ���� �ʰ� ������, ȣ���ص� �ƹ��͵� ���� �ʴ� null class���(��������� ���鶧 ���)
class NullImgui : public IImgui
{
public:
    virtual bool Initialize(ID3D12Device* device, DescriptorHeap* descriptorHeap,
        DXGI_FORMAT format, size_t srvOffset) override { return true; }
    virtual void AddComponent(IImguiComponent* comp) override {}
    virtual void RemoveComponent(IImguiComponent* comp) noexcept {}
    virtual void Render(ID3D12GraphicsCommandList* commandList) override {}
    virtual void PrepareRender() override {}
    virtual void Reset() override {}
};

class Imgui : public IImgui
{
public:
    Imgui(HWND hwnd);
    virtual ~Imgui();

    virtual bool Initialize(ID3D12Device* device, DescriptorHeap* descriptorHeap, 
        DXGI_FORMAT format, size_t srvOffset) override;
    virtual void AddComponent(IImguiComponent* comp) override;
    virtual void RemoveComponent(IImguiComponent* comp) noexcept override;
    virtual void Render(ID3D12GraphicsCommandList* commandList) override;
    virtual void PrepareRender() override;
    virtual void Reset() override;

private:
    HWND m_hwnd{};
    ImGuiIO* m_io;
    vector<IImguiComponent*> m_components;
};

