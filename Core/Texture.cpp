#include "pch.h"
#include "Texture.h"
#include "DeviceResources.h"
#include "Utility.h"

using namespace DirectX;
using Microsoft::WRL::ComPtr;

Texture::~Texture() = default;
Texture::Texture(ID3D12Device* device, DescriptorPile* descPile) noexcept :
    m_device{ device },
    m_descPile{ descPile }
{}

void Texture::Upload(ResourceUploadBatch* resUpload, const wstring& filename, size_t index)
{
    DX::ThrowIfFailed(
        CreateWICTextureFromFile(m_device, *resUpload, filename.c_str(), m_texture.ReleaseAndGetAddressOf()));

    CreateShaderResourceView(m_device, m_texture.Get(), m_descPile->GetCpuHandle(index));
    m_size = GetTextureSize(m_texture.Get());
    m_filename = filename;
    m_index = index;
}

void Texture::Draw(SpriteBatch* spriteBatch, const RECT& dest, const RECT* source)
{
    //�ؽ��� ũ��� �������� ��
    //dest�� ȭ�鿡 �����ִ� �簢��(ũ�Ⱑ �� ������ ������ �ø�)
    //source�� �ؽ��翡�� �������� �ȼ� �簢��
    //origin�� 0, 0���� ����. �߰����� ���� ��� �ø��� ��ġ�� ��߳�
    //�ؽ��簡 �þ�� �ؽ��簡 �������� ��� origin ������ ���������� ���ݾ� ��߳��� ������ ��������.
    //origin�� 0, 0 �� ������ ��ġ���� ����ؼ� �Ѱ��ִ� ������ �ؾ߰ڴ�.

    spriteBatch->Draw(m_descPile->GetGpuHandle(*m_index), m_size, dest, source, Colors::White, 0.f);
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
        ignore = WaitForSingleObjectEx(eventHandle, INFINITE, FALSE);
        CloseHandle(eventHandle);
    }

    return true;
}

static vector<vector<UINT32>> ConvertTo2D(UINT8* data, UINT width, UINT height, UINT rowPitch) 
{
    vector<vector<UINT32>> image(height, std::vector<UINT32>(width));
    for (UINT y: views::iota(0u, height))
    {
        for (UINT x: views::iota(0u, width))
        {
            UINT index = y * rowPitch + x * 4;
            image[y][x] = *reinterpret_cast<UINT32*>(&data[index]);
        }
    }
    return image;
}

static vector<Rectangle> FindRectangles(const D3D12_SUBRESOURCE_FOOTPRINT& footPrint, 
    const UINT32& bgColor, UINT8* data)
{
    int imageHeight = footPrint.Height;
    int imageWidth = footPrint.Width;

    vector<Rectangle> rectangles;
    vector<vector<bool>> visited(imageHeight, vector<bool>(imageWidth, false));

    auto image = ConvertTo2D(data, imageWidth, imageHeight, footPrint.RowPitch);

    auto isValid = [&](int x, int y) {
        return x >= 0 && x < imageWidth && y >= 0 && y < imageHeight;
        };

    for (UINT y : views::iota(0, imageHeight))
    {
        for (UINT x : views::iota(0, imageWidth))
        {
            if (!visited[y][x] && image[y][x] != bgColor)
            {
                // BFS ����
                int minX = x, minY = y, maxX = x, maxY = y;
                queue<pair<int, int>> q;
                q.push({ x, y });
                visited[y][x] = true;

                while (!q.empty())
                {
                    auto [cx, cy] = q.front();
                    q.pop();

                    minX = std::min(minX, cx);
                    minY = std::min(minY, cy);
                    maxX = std::max(maxX, cx);
                    maxY = std::max(maxY, cy);

                    // 4���� Ž��
                    const int dx[] = { -1, 1, 0, 0 };
                    const int dy[] = { 0, 0, -1, 1 };
                    for (int d : views::iota(0, 4))
                    {
                        int nx = cx + dx[d], ny = cy + dy[d];
                        if (isValid(nx, ny))
                        {
                            if (!visited[ny][nx] && image[ny][nx] != bgColor)
                            {
                                visited[ny][nx] = true;
                                q.push({ nx, ny });
                            }
                        }
                    }
                }

                int width = maxX - minX + 1;
                int height = maxY - minY + 1;
                rectangles.emplace_back(minX, minY, width, height);
            }
        }
    }

    return rectangles;
}

bool Texture::GetTextureAreaList(DX::DeviceResources* deviceRes, const UINT32& bgColor, vector<Rectangle>& outList)
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
 
    UINT8* data{ nullptr };
    readbackBuffer->Map(0, nullptr, reinterpret_cast<void**>(&data));
    outList = FindRectangles(layout.Footprint, bgColor, data);
    readbackBuffer->Unmap(0, nullptr);

    //�簢���� ��ø�� �ְų� ���������� ���� �׾��� �ִ� ��� ���� �̹������ �Ǵ� �� �� �ֵ��� ���ڵǰų� ���ԵǾ� �ִٸ� �����ش�.
    MergeRectangles(outList);

    return true;
}
