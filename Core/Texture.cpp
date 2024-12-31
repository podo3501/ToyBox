#include "pch.h"
#include "Texture.h"
#include "DeviceResources.h"

using namespace DirectX;

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

bool Texture::GetReadBackBuffer(DX::DeviceResources* deviceRes, ID3D12Resource** outReadbackBuffer)
{
    auto device = deviceRes->GetD3DDevice();
    auto commandList = deviceRes->GetCommandList();
    auto commandAllocator = deviceRes->GetCommandAllocator();
    auto commandQueue = deviceRes->GetCommandQueue();

    D3D12_PLACED_SUBRESOURCE_FOOTPRINT layout;
    UINT64 totalBytes = 0;

    // �ؽ�ó ���ҽ��� ���� ��������
    D3D12_RESOURCE_DESC textureDesc = m_texture->GetDesc();
    device->GetCopyableFootprints(&textureDesc, 0, 1, 0, &layout, nullptr, nullptr, &totalBytes);

    Microsoft::WRL::ComPtr<ID3D12Resource> readbackBuffer;
    CD3DX12_HEAP_PROPERTIES heapProps(D3D12_HEAP_TYPE_READBACK);
    CD3DX12_RESOURCE_DESC bufferDesc = CD3DX12_RESOURCE_DESC::Buffer(totalBytes);

    auto result = device->CreateCommittedResource(&heapProps, D3D12_HEAP_FLAG_NONE, &bufferDesc, 
        D3D12_RESOURCE_STATE_COPY_DEST, nullptr, IID_PPV_ARGS(&readbackBuffer));
    if (result != S_OK)
        return false;

    commandList->Reset(commandAllocator, nullptr);

    CD3DX12_TEXTURE_COPY_LOCATION destLocation(readbackBuffer.Get(), layout);
    CD3DX12_TEXTURE_COPY_LOCATION srcLocation(m_texture.Get(), 0);
    commandList->CopyTextureRegion(&destLocation, 0, 0, 0, &srcLocation, nullptr);

    commandList->Close();

    ID3D12CommandList* commandLists[] = { commandList };
    commandQueue->ExecuteCommandLists(_countof(commandLists), commandLists);

    //deviceRes->WaitForGpu();

    // GPU�� ����� �Ϸ��� ������ ���
    Microsoft::WRL::ComPtr<ID3D12Fence> fence;
    UINT64 fenceValue = 1;
    result = device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence));

    HANDLE fenceEvent = CreateEventEx(nullptr, nullptr, false, EVENT_ALL_ACCESS);

    result = commandQueue->Signal(fence.Get(), fenceValue);

    auto completed = fence->GetCompletedValue();
    result = fence->SetEventOnCompletion(fenceValue, fenceEvent);
    WaitForSingleObjectEx(fenceEvent, INFINITE, FALSE);
    //WaitForSingleObject(fenceEvent, INFINITE);
    CloseHandle(fenceEvent);

    //*outReadbackBuffer = readbackBuffer.Detach();

    UINT8* data;
    readbackBuffer->Map(0, nullptr, reinterpret_cast<void**>(&data));

    const XMINT2& pos{ 30, 30 };
    // �ȼ� ������ ��� (RGBA 4����Ʈ ����)
    UINT pixelOffset = pos.y * 64 * 4 + pos.x * 4;
    UINT32 pixelValue = *reinterpret_cast<UINT32*>(data + pixelOffset);

    UINT8 r = (pixelValue >> 16) & 0xFF; // R ä�� (���� 8��Ʈ)
    UINT8 g = (pixelValue >> 8) & 0xFF;  // G ä�� (���� 8��Ʈ)
    UINT8 b = pixelValue & 0xFF;         // B ä�� (���� 8��Ʈ)
    UINT8 a = (pixelValue >> 24) & 0xFF; // A ä�� (�ֻ��� 8��Ʈ)

    readbackBuffer->Unmap(0, nullptr);

    return true;
}