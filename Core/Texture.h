#pragma once

class Texture
{
public:
    Texture(ID3D12Device* device, DescriptorHeap* descHeap);
    void Upload(ResourceUploadBatch* resUpload, size_t descHeapIdx, const wstring& filename);

    XMUINT2 GetSize() const noexcept;
    void Reset();
    void Draw(SpriteBatch* spriteBatch, const XMUINT2& size, Vector2 screenPos, const XMFLOAT2& origin);
    
private:
    ID3D12Device* m_device;
    DescriptorHeap* m_descHeap;
    Microsoft::WRL::ComPtr<ID3D12Resource> m_texture;
    size_t m_descHeapIdx{ 0 };
};

