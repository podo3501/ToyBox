#include "pch.h"
#include "Texture.h"
#include "DeviceResources.h"
#include "Utility.h"

using namespace DirectX;
using Microsoft::WRL::ComPtr;

Texture::Texture() noexcept = default;
Texture::~Texture() = default;
Texture::Texture(const Texture* tex, const Rectangle* rect) noexcept
{
    m_texture = tex->m_texture;
    m_size = tex->m_size;
    m_descHeapIdx = tex->m_descHeapIdx;
    m_filename = tex->m_filename;

    SetRectangle(rect);
}

void Texture::Upload(ID3D12Device* device, DescriptorHeap* descHeap, ResourceUploadBatch* resUpload, 
    const std::wstring& filename, const Rectangle* rect, std::size_t descHeapIdx)
{
    DX::ThrowIfFailed(
        CreateWICTextureFromFile(device, *resUpload, filename.c_str(), m_texture.ReleaseAndGetAddressOf()));
    
    CreateShaderResourceView(device, m_texture.Get(), descHeap->GetCpuHandle(descHeapIdx));
    m_size = GetTextureSize(m_texture.Get());
    m_descHeapIdx = descHeapIdx;
    m_filename = filename;

    SetRectangle(rect);
}

void Texture::SetRectangle(const Rectangle* rect) noexcept
{
    Rectangle curRect = { 0, 0, static_cast<long>(m_size.x), static_cast<long>(m_size.y) };

    if (rect == nullptr)
    {
        m_fullSize = true;
        m_rect = curRect;
    }
    else
    {
        if (curRect == *rect)
            m_fullSize = true;
        m_rect = *rect;
    }
}

void Texture::Draw(SpriteBatch* spriteBatch, const DescriptorHeap* descHeap, const RECT& dest, const RECT* source)
{
    //�ؽ��� ũ��� �������� ��
    //dest�� ȭ�鿡 �����ִ� �簢��(ũ�Ⱑ �� ������ ������ �ø�)
    //source�� �ؽ��翡�� �������� �ȼ� �簢��
    //origin�� 0, 0���� ����. �߰����� ���� ��� �ø��� ��ġ�� ��߳�
    //�ؽ��簡 �þ�� �ؽ��簡 �������� ��� origin ������ ���������� ���ݾ� ��߳��� ������ ��������.
    //origin�� 0, 0 �� ������ ��ġ���� ����ؼ� �Ѱ��ִ� ������ �ؾ߰ڴ�.

    spriteBatch->Draw(descHeap->GetGpuHandle(m_descHeapIdx), m_size, dest, source, Colors::White, 0.f);
}

void Texture::Reset() 
{ 
    m_texture.Reset(); 
}

static bool CreateReadbackBuffer(ID3D12Device* device, UINT64 totalBytes, ComPtr<ID3D12Resource>& outReadbackBuffer)
{
    CD3DX12_HEAP_PROPERTIES heapProps(D3D12_HEAP_TYPE_READBACK);
    CD3DX12_RESOURCE_DESC bufferDesc = CD3DX12_RESOURCE_DESC::Buffer(totalBytes);

    ReturnIfFailed(device->CreateCommittedResource(
        &heapProps,
        D3D12_HEAP_FLAG_NONE,
        &bufferDesc,
        D3D12_RESOURCE_STATE_COPY_DEST,
        nullptr,
        IID_PPV_ARGS(&outReadbackBuffer)));

    return true;
}

static void CopyTextureToBuffer(ID3D12GraphicsCommandList* commandList, ID3D12Resource* texture,
    const D3D12_PLACED_SUBRESOURCE_FOOTPRINT& layout, ComPtr<ID3D12Resource> readbackBuffer)
{
    TransitionResource(commandList, texture,
        D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, D3D12_RESOURCE_STATE_COPY_SOURCE);

    CD3DX12_TEXTURE_COPY_LOCATION destLocation(readbackBuffer.Get(), layout);
    CD3DX12_TEXTURE_COPY_LOCATION srcLocation(texture, 0);
    commandList->CopyTextureRegion(&destLocation, 0, 0, 0, &srcLocation, nullptr);

    // �ؽ�ó ���ҽ� ���� ����
    TransitionResource(commandList, texture,
        D3D12_RESOURCE_STATE_COPY_SOURCE, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
}

static bool WaitForGpuWork(ID3D12Device* device, ID3D12CommandQueue* commandQueue)
{
    Microsoft::WRL::ComPtr<ID3D12Fence> fence;
    UINT64 fenceValue = 1;

    ReturnIfFailed(device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence)));
    ReturnIfFailed(commandQueue->Signal(fence.Get(), fenceValue));
    
    if (fence->GetCompletedValue() < fenceValue) // GPU �۾� �Ϸ� ���
    {
        HANDLE eventHandle = CreateEventEx(nullptr, nullptr, false, EVENT_ALL_ACCESS);
        if (eventHandle == nullptr) return false;

        fence->SetEventOnCompletion(fenceValue, eventHandle);
        std::ignore = WaitForSingleObjectEx(eventHandle, INFINITE, FALSE);
        CloseHandle(eventHandle);
    }

    return true;
}

bool Texture::GetReadBackBuffer(DX::DeviceResources* deviceRes, ID3D12Resource** outReadbackBuffer,
    D3D12_PLACED_SUBRESOURCE_FOOTPRINT* outLayout)
{
    auto device = deviceRes->GetD3DDevice();
    auto commandList = deviceRes->GetCommandList();
    auto commandAllocator = deviceRes->GetCommandAllocator();
    auto commandQueue = deviceRes->GetCommandQueue();

    // �ؽ�ó ���ҽ��� ���� ��������
    D3D12_RESOURCE_DESC textureDesc = m_texture->GetDesc();
    D3D12_PLACED_SUBRESOURCE_FOOTPRINT layout{};
    UINT64 totalBytes{ 0 }, rowPitch{ 0 };

    device->GetCopyableFootprints(&textureDesc, 0, 1, 0, &layout, nullptr, &rowPitch, &totalBytes);

    Microsoft::WRL::ComPtr<ID3D12Resource> readbackBuffer;
    ReturnIfFalse(CreateReadbackBuffer(device, totalBytes, readbackBuffer));

    ReturnIfFailed(commandList->Reset(commandAllocator, nullptr));

    // �ؽ�ó�� �����ϴ� ��ɾ� ����
    CopyTextureToBuffer(commandList, m_texture.Get(), layout, readbackBuffer);

    // ��� ����Ʈ ����
    ReturnIfFailed(commandList->Close());
    ID3D12CommandList* commandLists[] = { commandList };
    commandQueue->ExecuteCommandLists(_countof(commandLists), commandLists);

    // GPU �۾� �Ϸ� ���
    ReturnIfFalse(WaitForGpuWork(device, commandQueue));

    // ��� �Ű������� �о�� ���� ����
    *outReadbackBuffer = readbackBuffer.Detach();
    if (outLayout) *outLayout = layout;

    return true;
}
