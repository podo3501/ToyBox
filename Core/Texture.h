#pragma once

namespace DX
{
    class DeviceResources;
}

class Texture
{
public:
    Texture() = delete;
    Texture(ID3D12Device* device, DescriptorPile* descPile) noexcept;
    ~Texture();

    void Reset();
    void Upload(ResourceUploadBatch* resUpload, const wstring& filename, size_t index);
    void Draw(SpriteBatch* spriteBatch, const RECT& dest, const RECT* source);
    bool GetTextureAreaList(DX::DeviceResources* deviceRes, const UINT32& bgColor, vector<Rectangle>& outList);

    inline XMUINT2 GetSize() const noexcept { return m_size; }
    inline size_t GetIndex() const noexcept { return *m_index; }
    inline const std::wstring& GetFilename() const noexcept;
    
private:
    ID3D12Device* m_device;
    DescriptorPile* m_descPile;
    Microsoft::WRL::ComPtr<ID3D12Resource> m_texture;
    XMUINT2 m_size{};
    optional<size_t> m_index;
    std::wstring m_filename{};
};

const std::wstring& Texture::GetFilename() const noexcept { return m_filename; }

