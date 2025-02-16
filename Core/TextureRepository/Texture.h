#pragma once
#include "TextureResource.h"

namespace DX
{
    class DeviceResources;
}

class Texture : public TextureResource
{
public:
    static TextureResourceID GetTypeStatic() { return TextureResourceID::Texture; }
    virtual TextureResourceID GetTypeID() const noexcept override { return GetTypeStatic(); }

    Texture() = delete;
    Texture(ID3D12Device* device, DescriptorHeap* descHeap) noexcept;
    ~Texture();

    void Reset();
    void Load(ResourceUploadBatch* resUpload, const wstring& filename, size_t index);
    void Draw(SpriteBatch* spriteBatch, const RECT& dest, const RECT* source);

    bool ExtractTextureAreas(DX::DeviceResources* deviceRes, const UINT32& bgColor, vector<Rectangle>& outList);
    inline ID3D12Resource* GetResource() const noexcept { return m_texture.Get(); }
    inline XMUINT2 GetSize() const noexcept { return m_size; }
    
private:
    Microsoft::WRL::ComPtr<ID3D12Resource> m_texture;
    XMUINT2 m_size{};
};

