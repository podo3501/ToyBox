#include "pch.h"
#include "Texture.h"
#include "TextureHelper.h"
#include "../Utility.h"

using namespace DirectX;
using Microsoft::WRL::ComPtr;

Texture::~Texture() = default;
Texture::Texture(ID3D12Device* device, DescriptorHeap* descHeap) noexcept :
    TextureResource{ device, descHeap }
{}

void Texture::Load(ResourceUploadBatch* resUpload, const wstring& filename, size_t index)
{
    DX::ThrowIfFailed(
        CreateWICTextureFromFile(m_device, *resUpload, filename.c_str(), m_texture.ReleaseAndGetAddressOf()));

    CreateShaderResourceView(m_device, m_texture.Get(), m_srvDescriptors->GetCpuHandle(index));
    m_size = GetTextureSize(m_texture.Get());
    SetFilename(filename);
    SetIndex(index);
}

void Texture::Draw(SpriteBatch* spriteBatch, const RECT& dest, const RECT* source)
{
    //�ؽ��� ũ��� �������� ��
    //dest�� ȭ�鿡 �����ִ� �簢��(ũ�Ⱑ �� ������ ������ �ø�)
    //source�� �ؽ��翡�� �������� �ȼ� �簢��
    //origin�� 0, 0���� ����. �߰����� ���� ��� �ø��� ��ġ�� ��߳�
    //�ؽ��簡 �þ�� �ؽ��簡 �������� ��� origin ������ ���������� ���ݾ� ��߳��� ������ ��������.
    //origin�� 0, 0 �� ������ ��ġ���� ����ؼ� �Ѱ��ִ� ������ �ؾ߰ڴ�.

    spriteBatch->Draw(m_srvDescriptors->GetGpuHandle(GetIndex()), m_size, dest, source, Colors::White, 0.f);
}

void Texture::Reset() 
{ 
    m_texture.Reset(); 
}

bool Texture::ExtractTextureAreas(DX::DeviceResources* deviceRes, const UINT32& bgColor, vector<Rectangle>& outList)
{
    return ExtractAreas(deviceRes, m_texture.Get(), bgColor, outList);
}
