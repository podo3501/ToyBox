#include "pch.h"
#include "Texture.h"

using namespace DirectX;

Texture::~Texture() = default;
Texture::Texture(ID3D12Device* device, DescriptorHeap* descHeap) noexcept :
    m_device{ device }, m_descHeap{ descHeap } {}
Texture::Texture(const Texture* tex, const Rectangle* rect) noexcept
{
    m_device = tex->m_device;
    m_descHeap = tex->m_descHeap;
    m_texture = tex->m_texture;
    m_size = tex->m_size;
    m_descHeapIdx = tex->m_descHeapIdx;
    m_filename = tex->m_filename;

    SetRectangle(rect);
}

void Texture::Upload(ResourceUploadBatch* resUpload, const std::wstring& filename, 
    const Rectangle* rect, std::size_t descHeapIdx)
{
    DX::ThrowIfFailed(
        CreateWICTextureFromFile(m_device, *resUpload, filename.c_str(), m_texture.ReleaseAndGetAddressOf()));
    CreateShaderResourceView(m_device, m_texture.Get(), m_descHeap->GetCpuHandle(descHeapIdx));
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

void Texture::Draw(SpriteBatch* spriteBatch, const RECT& dest, const RECT* source)
{
    //�ؽ��� ũ��� �������� ��
    //dest�� ȭ�鿡 �����ִ� �簢��(ũ�Ⱑ �� ������ ������ �ø�)
    //source�� �ؽ��翡�� �������� �ȼ� �簢��
    //origin�� 0, 0���� ����. �߰����� ���� ��� �ø��� ��ġ�� ��߳�
    //�ؽ��簡 �þ�� �ؽ��簡 �������� ��� origin ������ ���������� ���ݾ� ��߳��� ������ ��������.
    //origin�� 0, 0 �� ������ ��ġ���� ����ؼ� �Ѱ��ִ� ������ �ؾ߰ڴ�.
    spriteBatch->Draw(m_descHeap->GetGpuHandle(m_descHeapIdx), m_size, dest, source, Colors::White, 0.f);
}

void Texture::Reset() 
{ 
    m_texture.Reset(); 
}