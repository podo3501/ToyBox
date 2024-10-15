#include "pch.h"
#include "Texture.h"

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