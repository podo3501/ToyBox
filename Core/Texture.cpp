#include "pch.h"
#include "Texture.h"

using namespace DirectX;

Texture::Texture(ID3D12Device* device, DescriptorHeap* descHeap) :
    m_device{ device }, m_descHeap{ descHeap } {}

void Texture::Upload(ResourceUploadBatch* resUpload, const std::wstring& filename, 
    const Rectangle* rect, std::size_t descHeapIdx)
{
    DX::ThrowIfFailed(
        CreateWICTextureFromFile(m_device, *resUpload, filename.c_str(), m_texture.ReleaseAndGetAddressOf()));
    CreateShaderResourceView(m_device, m_texture.Get(), m_descHeap->GetCpuHandle(descHeapIdx));
    m_descHeapIdx = descHeapIdx;
    m_filename = filename;

    SetRectangle(rect);
}

void Texture::Set(const Microsoft::WRL::ComPtr<ID3D12Resource>& texture, const std::wstring& filename, 
    const Rectangle* rect, std::size_t descHeapIdx)
{
    m_texture = texture;
    m_descHeapIdx = descHeapIdx;
    m_filename = filename;

    SetRectangle(rect);
}

void Texture::SetRectangle(const Rectangle* rect)
{
    const XMUINT2& texSize = GetTextureSize(m_texture.Get());
    Rectangle curRect = { 0, 0, static_cast<long>(texSize.x), static_cast<long>(texSize.y) };

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
    
    //RECT rect(0, 24, 70, 48);
    XMUINT2 newSize(size);
    XMFLOAT2 scale{ 1.f, 1.f };
    if (newSize.x == 82 && newSize.y == 48)
    {
        newSize.x = 48;
        scale.x = 82.f / 48.f;
        //scale.x = -4.f;
    }
    //XMFLOAT2 newOrigin{ 0.f, 0.f };
    //spriteBatch->Draw(m_descHeap->GetGpuHandle(m_descHeapIdx), newSize,
       // screenPos, nullptr, Colors::White, 0.f, newOrigin, scale);
    //텍스춰 크기는 고정으로 함
    //dest는 화면에 보여주는 사각형(크기가 안 맞으면 강제로 늘림)
    //source는 텍스춰에서 가져오는 픽셀 사각형
    //origin은 0, 0으로 고정. 중간으로 했을 경우 늘릴때 위치가 어긋남
    //origin 값을 주면서도 제대로 나오는 방법이 뭐가 있을까
    RECT sourceRect = { 14, 24, 24, 48 };
    RECT dest{ long(screenPos.x - origin.x), long(screenPos.y - origin.y), long(screenPos.x + size.x - origin.x), long(screenPos.y + size.y - origin.y) };
    spriteBatch->Draw(m_descHeap->GetGpuHandle(m_descHeapIdx), newSize,
        dest, &sourceRect, Colors::White, 0.f);
}

void Texture::Reset() 
{ 
    m_texture.Reset(); 
}