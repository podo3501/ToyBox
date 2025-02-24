#include "pch.h"
#include "TextureRenderTarget.h"
#include "../DeviceResources.h"
#include "../Include/IComponent.h"
#include "../Utility.h"

constexpr FLOAT ClearColor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };

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

bool TextureRenderTarget::Create(DXGI_FORMAT texFormat, 
    const XMUINT2& size, const XMINT2& pos, size_t offset, IComponent* component)
{
    m_component = component;
    SetIndex(offset);
    m_position = pos;
    return CreateTextureResource(texFormat, size);
}

bool TextureRenderTarget::ModifyRenderTexture(const XMUINT2& size)
{
    m_deviceResources->WaitForGpu();
    return CreateTextureResource(m_resDesc.Format, size);
}

bool TextureRenderTarget::CreateTextureResource(DXGI_FORMAT texFormat, const XMUINT2& size)
{
    //렌더링할 캔버스(텍스쳐)를 만든다.
    m_resDesc = GetResourceDesc(texFormat, size);
    const CD3DX12_HEAP_PROPERTIES heapProperties(D3D12_HEAP_TYPE_DEFAULT);
    D3D12_CLEAR_VALUE clear = GetClearValue();

    ReturnIfFailed(m_device->CreateCommittedResource(
        &heapProperties,
        D3D12_HEAP_FLAG_NONE,
        &m_resDesc,
        D3D12_RESOURCE_STATE_GENERIC_READ,
        &clear,
        IID_PPV_ARGS(m_texResource.ReleaseAndGetAddressOf())
    ));

    CreateRtvAndSrv(m_texResource.Get());
    SetSize(size);

    return true;
}

void TextureRenderTarget::Render(ID3D12GraphicsCommandList* commandList, ITextureRender* renderer, SpriteBatch* sprite)
{
    if (m_component == nullptr)
        return;

    //속도를 위해서 필요한 부분을 제외한 다른 부분은 잘라낸다.
    //D3D12_RECT scissorRect{};
    //scissorRect.left = 0;
    //scissorRect.top = 0;
    //scissorRect.right = static_cast<long>(m_resDesc.Width);
    //scissorRect.bottom = static_cast<long>(m_resDesc.Height);
    //commandList->RSSetScissorRects(1, &scissorRect);

    TransitionResource(commandList, m_texResource.Get(),
        D3D12_RESOURCE_STATE_GENERIC_READ,
        D3D12_RESOURCE_STATE_RENDER_TARGET);

    CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(m_rtvDescriptor->GetFirstCpuHandle());
    commandList->OMSetRenderTargets(1, &rtvHandle, FALSE, nullptr);
    commandList->ClearRenderTargetView(rtvHandle, ClearColor, 0, nullptr);

    //찍는것은 위치값을 0, 0로 옮긴다. 100, 100에서 찍는다면 텍스쳐는 좌표가 0, 0부터 시작하는게 고정이기 때문에 낭비가 된다.
    XMMATRIX transform = XMMatrixTranslation(-static_cast<float>(m_position.x), -static_cast<float>(m_position.y), 0.0f);

    sprite->Begin(commandList, SpriteSortMode_Deferred, transform);
    m_component->ProcessRender(renderer);
    sprite->End();

    TransitionResource(commandList, m_texResource.Get(),
        D3D12_RESOURCE_STATE_RENDER_TARGET, 
        D3D12_RESOURCE_STATE_GENERIC_READ);
}

