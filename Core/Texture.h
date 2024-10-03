#pragma once

class Texture
{
public:
    Texture(ID3D12Device* device, DirectX::DescriptorHeap* descHeap);
    void Upload(DirectX::ResourceUploadBatch* resUpload, std::uint32_t descHeapIdx, const std::wstring& filename);

    DirectX::XMUINT2 GetSize() const noexcept;
    void Reset();
    void Draw(DirectX::SpriteBatch* spriteBatch, DirectX::SimpleMath::Vector2 screenPos, const DirectX::XMFLOAT2& origin);
    
private:
    ID3D12Device* m_device;
    DirectX::DescriptorHeap* m_descHeap;
    Microsoft::WRL::ComPtr<ID3D12Resource> m_texture;
    std::uint32_t m_descHeapIdx{ 0 };
};

