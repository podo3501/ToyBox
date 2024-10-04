#include "pch.h"
#include "Texture.h"

using namespace DirectX;

Texture::Texture(ID3D12Device* device, DescriptorHeap* descHeap) :
    m_device{ device }, m_descHeap{ descHeap } {}

void Texture::Upload(ResourceUploadBatch* resUpload, std::uint32_t descHeapIdx, const std::wstring& filename)
{
    DX::ThrowIfFailed(
        CreateWICTextureFromFile(m_device, *resUpload, filename.c_str(), m_texture.ReleaseAndGetAddressOf()));
    CreateShaderResourceView(m_device, m_texture.Get(), m_descHeap->GetCpuHandle(descHeapIdx));
    m_descHeapIdx = descHeapIdx;
}

XMUINT2 Texture::GetSize() const noexcept
{
    return GetTextureSize(m_texture.Get());
}

void Texture::Reset()
{
    m_texture.Reset();
}

void Texture::Draw(SpriteBatch* spriteBatch, const XMUINT2& size, Vector2 screenPos, const XMFLOAT2& origin)
{
    //auto size = GetSize();
    //RECT rect = { 0, 0, static_cast<LONG>(size.x / 2), static_cast<LONG>(size.y / 2) };
    //RECT rect = { 0, 0, static_cast<LONG>(size.x), static_cast<LONG>(size.y) };
    //spriteBatch->Draw(m_descHeap->GetGpuHandle(m_descHeapIdx), size,
       // screenPos, &rect, Colors::White, 0.f, { float(size.x / 2), float(size.y / 2) });
    /*auto size = GetSize();
    spriteBatch->Draw(m_descHeap->GetGpuHandle(m_descHeapIdx), size,
        screenPos, nullptr, Colors::White, 0.f, { float(size.x / 2), float(size.y / 2) });*/
    //auto size = GetSize();
    //size += 30;
    spriteBatch->Draw(m_descHeap->GetGpuHandle(m_descHeapIdx), size,
        screenPos, nullptr, Colors::White, 0.f, origin);
}