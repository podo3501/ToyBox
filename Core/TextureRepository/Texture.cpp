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
    //텍스춰 크기는 고정으로 함
    //dest는 화면에 보여주는 사각형(크기가 안 맞으면 강제로 늘림)
    //source는 텍스춰에서 가져오는 픽셀 사각형
    //origin은 0, 0으로 고정. 중간으로 했을 경우 늘릴때 위치가 어긋남
    //텍스춰가 늘어나면 텍스춰가 여러장일 경우 origin 값으로 설정했을때 조금씩 어긋나는 현상이 벌어진다.
    //origin을 0, 0 로 고정후 위치값을 계산해서 넘겨주는 식으로 해야겠다.

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
