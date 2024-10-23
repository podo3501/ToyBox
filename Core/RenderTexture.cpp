#include "pch.h"
#include "../Include/IRenderItem.h"
#include "RenderTexture.h"
#include "Utility.h"

constexpr FLOAT ClearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };

RenderTexture::~RenderTexture() = default;
RenderTexture::RenderTexture(ID3D12Device* device, DescriptorHeap* srvDescriptor) :
    m_device(device),
    m_srvDescriptor{ srvDescriptor },
    m_renderTargetTexture{ nullptr },
    m_srvHandle{},
    m_renderItem{ nullptr }
{
    m_rtvDescriptor = make_unique<DescriptorHeap>(device,
        D3D12_DESCRIPTOR_HEAP_TYPE_RTV,
        D3D12_DESCRIPTOR_HEAP_FLAG_NONE, 1);
}

bool RenderTexture::Create(DXGI_FORMAT texFormat, XMUINT2 size, size_t offset, IRenderItem* renderItem)
{
    m_renderItem = renderItem;
    //화면을 저장할 Texture를 만든다.
    const CD3DX12_HEAP_PROPERTIES renderTextureHeapProperties(D3D12_HEAP_TYPE_DEFAULT);
    D3D12_RESOURCE_DESC renderTextureDesc = CD3DX12_RESOURCE_DESC::Tex2D(
        texFormat,
        size.x, size.y,
        1, // This depth stencil view has only one texture.
        1  // Use a single mipmap level.
    );
    renderTextureDesc.Flags |= D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;

    D3D12_CLEAR_VALUE optClear{ texFormat, { ClearColor[0], ClearColor[1], ClearColor[2], ClearColor[3] } };
    
    ReturnIfFailed(m_device->CreateCommittedResource(
        &renderTextureHeapProperties,
        D3D12_HEAP_FLAG_NONE,
        &renderTextureDesc,
        D3D12_RESOURCE_STATE_RENDER_TARGET,
        &optClear,
        IID_PPV_ARGS(&m_renderTargetTexture)
    ));

    CreateRenderTargetView(m_device, m_renderTargetTexture.Get(), m_rtvDescriptor->GetFirstCpuHandle());

    //텍스춰를 읽기 위해서 Srv로 만든다.
    CreateShaderResourceView(m_device, m_renderTargetTexture.Get(), m_srvDescriptor->GetCpuHandle(offset));
    m_srvHandle = m_srvDescriptor->GetGpuHandle(offset);

    return true;
}

void RenderTexture::Render(ID3D12GraphicsCommandList* commandList, IRender* renderer, SpriteBatch* sprite, IRenderItem* renderItem)
{
    // 클라이언트 화면을 렌더링할 렌더 타겟 뷰 설정
    CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(m_rtvDescriptor->GetFirstCpuHandle());
    commandList->OMSetRenderTargets(1, &rtvHandle, FALSE, nullptr);
    commandList->ClearRenderTargetView(rtvHandle, ClearColor, 0, nullptr);

    sprite->Begin(commandList);

    /*if (!m_renderItem)
        renderItem->Render(renderer);
    else*/
        m_renderItem->Render(renderer);

    sprite->End();
}