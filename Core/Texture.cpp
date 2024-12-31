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
    //텍스춰 크기는 고정으로 함
    //dest는 화면에 보여주는 사각형(크기가 안 맞으면 강제로 늘림)
    //source는 텍스춰에서 가져오는 픽셀 사각형
    //origin은 0, 0으로 고정. 중간으로 했을 경우 늘릴때 위치가 어긋남
    //텍스춰가 늘어나면 텍스춰가 여러장일 경우 origin 값으로 설정했을때 조금씩 어긋나는 현상이 벌어진다.
    //origin을 0, 0 로 고정후 위치값을 계산해서 넘겨주는 식으로 해야겠다.

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

    // 텍스처 리소스의 설명 가져오기
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

    // GPU가 명령을 완료할 때까지 대기
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
    // 픽셀 오프셋 계산 (RGBA 4바이트 기준)
    UINT pixelOffset = pos.y * 64 * 4 + pos.x * 4;
    UINT32 pixelValue = *reinterpret_cast<UINT32*>(data + pixelOffset);

    UINT8 r = (pixelValue >> 16) & 0xFF; // R 채널 (상위 8비트)
    UINT8 g = (pixelValue >> 8) & 0xFF;  // G 채널 (다음 8비트)
    UINT8 b = pixelValue & 0xFF;         // B 채널 (하위 8비트)
    UINT8 a = (pixelValue >> 24) & 0xFF; // A 채널 (최상위 8비트)

    readbackBuffer->Unmap(0, nullptr);

    return true;
}