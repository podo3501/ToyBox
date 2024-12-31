#include "pch.h"
#include "../Include/IComponent.h"
#include "RenderTexture.h"
#include "Utility.h"

constexpr FLOAT ClearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };

RenderTexture::~RenderTexture()
{
}

RenderTexture::RenderTexture(ID3D12Device* device, DescriptorHeap* srvDescriptor) :
    m_device(device),
    m_srvDescriptor{ srvDescriptor },
    m_component{ nullptr }
{
    m_rtvDescriptor = make_unique<DescriptorHeap>(device,
        D3D12_DESCRIPTOR_HEAP_TYPE_RTV,
        D3D12_DESCRIPTOR_HEAP_FLAG_NONE, 1);
}

D3D12_CLEAR_VALUE RenderTexture::GetClearValue() const noexcept
{
    return { m_resDesc.Format, { ClearColor[0], ClearColor[1], ClearColor[2], ClearColor[3] } };
}

D3D12_RESOURCE_DESC RenderTexture::GetResourceDesc(DXGI_FORMAT format, const XMUINT2& size) const noexcept
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

//새로운 텍스춰를 만들어야 하는데 같은 ID3D12Resource 변수를 지정할 경우 graphic card에서는 아직
//사용하고 있어서 안된다. 그래서 변수를 몇개 만들어서 돌아가면서 사용함
inline Microsoft::WRL::ComPtr<ID3D12Resource>& RenderTexture::GetTextureResource() noexcept
{
    if (m_renderTargetIndex >= RenderTargetCount)
        m_renderTargetIndex = 0;
    return m_renderTargetTexture[m_renderTargetIndex++];
}

void RenderTexture::CreateRtvAndSrv(ID3D12Resource* resource)
{
    CreateRenderTargetView(m_device, resource, m_rtvDescriptor->GetFirstCpuHandle());
    CreateShaderResourceView(m_device, resource, m_srvDescriptor->GetCpuHandle(m_offset));
}

bool RenderTexture::Create(DXGI_FORMAT texFormat, XMUINT2 size, size_t offset, IComponent* component)
{
    m_component = component;
    m_offset = offset;
    //화면을 저장할 Texture를 만든다.
    m_resDesc = GetResourceDesc(texFormat, size);
    auto& texResource = GetTextureResource();
    const CD3DX12_HEAP_PROPERTIES renderTextureHeapProperties(D3D12_HEAP_TYPE_DEFAULT);
    D3D12_CLEAR_VALUE clear = GetClearValue();

    ReturnIfFailed(m_device->CreateCommittedResource(
        &renderTextureHeapProperties,
        D3D12_HEAP_FLAG_NONE,
        &m_resDesc,
        D3D12_RESOURCE_STATE_RENDER_TARGET,
        &clear,
        IID_PPV_ARGS(texResource.ReleaseAndGetAddressOf())
    ));

    CreateRtvAndSrv(texResource.Get());

    return true;
}

bool RenderTexture::ModifyRenderTexture(const XMUINT2& size)
{
    m_resDesc = GetResourceDesc(m_resDesc.Format, size);
    auto& texResource = GetTextureResource();
    const CD3DX12_HEAP_PROPERTIES renderTextureHeapProperties(D3D12_HEAP_TYPE_DEFAULT);
    D3D12_CLEAR_VALUE clear = GetClearValue();

    ReturnIfFailed(m_device->CreateCommittedResource(
        &renderTextureHeapProperties,
        D3D12_HEAP_FLAG_NONE,
        &m_resDesc,
        D3D12_RESOURCE_STATE_RENDER_TARGET,
        &clear,
        IID_PPV_ARGS(texResource.ReleaseAndGetAddressOf())
    ));

    CreateRtvAndSrv(texResource.Get());

    return true;
}

void RenderTexture::Render(ID3D12GraphicsCommandList* commandList, IRender* renderer, SpriteBatch* sprite)
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

    //m_scene->RenderScene(renderer);
    m_component->ProcessRender(renderer);

    sprite->End();
}

ImTextureID RenderTexture::GetTextureID() const noexcept
{ 
    return static_cast<ImTextureID>(m_srvDescriptor->GetGpuHandle(m_offset).ptr);
}