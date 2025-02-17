#include "pch.h"
#include "TextureRenderTarget.h"
#include "../DeviceResources.h"
#include "../Include/IComponent.h"
#include "../Utility.h"

constexpr FLOAT ClearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };

TextureRenderTarget::~TextureRenderTarget()
{}

TextureRenderTarget::TextureRenderTarget(DX::DeviceResources* deviceResources, DescriptorHeap* descHeap) :
    TextureResource{ deviceResources->GetD3DDevice(), descHeap },
    m_deviceResources{ deviceResources },
    m_component{ nullptr }
{
    m_rtvDescriptor = make_unique<DescriptorHeap>(m_device,
        D3D12_DESCRIPTOR_HEAP_TYPE_RTV,
        D3D12_DESCRIPTOR_HEAP_FLAG_NONE, 1);
}

D3D12_CLEAR_VALUE TextureRenderTarget::GetClearValue() const noexcept
{
    return { m_resDesc.Format, { ClearColor[0], ClearColor[1], ClearColor[2], ClearColor[3] } };
}

D3D12_RESOURCE_DESC TextureRenderTarget::GetResourceDesc(DXGI_FORMAT format, const XMUINT2& size) const noexcept
{
    D3D12_RESOURCE_DESC resDesc = CD3DX12_RESOURCE_DESC::Tex2D(
        format,
        size.x, size.y,
        1, // This depth stencil view has only one texture.
        1  // Use a single mipmap level.
    );
    resDesc.Flags |= D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;

    return resDesc;
}

void TextureRenderTarget::CreateRtvAndSrv(ID3D12Resource* resource)
{
    CreateRenderTargetView(m_device, resource, m_rtvDescriptor->GetFirstCpuHandle());
    CreateShaderResourceView(m_device, resource, m_srvDescriptors->GetCpuHandle(GetIndex()));
}

bool TextureRenderTarget::Create(DXGI_FORMAT texFormat, XMUINT2 size, size_t offset, IComponent* component)
{
    m_component = component;
    SetIndex(offset);
    //화면을 저장할 Texture를 만든다.
    m_resDesc = GetResourceDesc(texFormat, size);
    const CD3DX12_HEAP_PROPERTIES TextureRenderTargetHeapProperties(D3D12_HEAP_TYPE_DEFAULT);
    D3D12_CLEAR_VALUE clear = GetClearValue();

    ReturnIfFailed(m_device->CreateCommittedResource(
        &TextureRenderTargetHeapProperties,
        D3D12_HEAP_FLAG_NONE,
        &m_resDesc,
        D3D12_RESOURCE_STATE_RENDER_TARGET,
        &clear,
        IID_PPV_ARGS(m_texResource.ReleaseAndGetAddressOf())
    ));

    CreateRtvAndSrv(m_texResource.Get());

    return true;
}

bool TextureRenderTarget::ModifyRenderTexture(const XMUINT2& size)
{
    m_deviceResources->WaitForGpu();

    m_resDesc = GetResourceDesc(m_resDesc.Format, size);
    const CD3DX12_HEAP_PROPERTIES TextureRenderTargetHeapProperties(D3D12_HEAP_TYPE_DEFAULT);
    D3D12_CLEAR_VALUE clear = GetClearValue();

    ReturnIfFailed(m_device->CreateCommittedResource(
        &TextureRenderTargetHeapProperties,
        D3D12_HEAP_FLAG_NONE,
        &m_resDesc,
        D3D12_RESOURCE_STATE_RENDER_TARGET,
        &clear,
        IID_PPV_ARGS(m_texResource.ReleaseAndGetAddressOf())
    ));

    CreateRtvAndSrv(m_texResource.Get());

    return true;
}

void TextureRenderTarget::Render(ID3D12GraphicsCommandList* commandList, ITextureRender* renderer, SpriteBatch* sprite)
{
    if (m_component == nullptr)
        return;

    //D3D12_VIEWPORT viewport{};
    //viewport.Width = static_cast<float>(m_size.x);
    //viewport.Height = static_cast<float>(m_size.y);
    //viewport.MaxDepth = 1.0f;

    //D3D12_RECT scissorRect{};
    //scissorRect.right = static_cast<long>(m_size.x);
    //scissorRect.bottom = static_cast<long>(m_size.y);

    //commandList->RSSetViewports(1, &viewport);
    //commandList->RSSetScissorRects(1, &scissorRect);

    // 클라이언트 화면을 렌더링할 렌더 타겟 뷰 설정
    CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(m_rtvDescriptor->GetFirstCpuHandle());
    commandList->OMSetRenderTargets(1, &rtvHandle, FALSE, nullptr);
    commandList->ClearRenderTargetView(rtvHandle, ClearColor, 0, nullptr);

    sprite->Begin(commandList);
    m_component->ProcessRender(renderer);
    sprite->End();
}

