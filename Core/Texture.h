#pragma once

class Texture
{
public:
    Texture() noexcept;
    Texture(const Texture* tex, const Rectangle* rect) noexcept;
    ~Texture();

    void Upload(ID3D12Device* device, DescriptorHeap* descHeap, ResourceUploadBatch* resUpload, const std::wstring& filename, const Rectangle* rect, std::size_t descHeapIdx);
    void Draw(SpriteBatch* spriteBatch, const DescriptorHeap* descHeap, const RECT& dest, const RECT* source, bool selected);
    void Reset();

    inline XMUINT2 GetSize() const noexcept;
    inline const std::wstring& GetFilename() const noexcept;
    inline const Rectangle& GetRect() const noexcept;
    inline bool IsFullSize() const noexcept;
    
private:
    void SetRectangle(const Rectangle* rect) noexcept;

    Microsoft::WRL::ComPtr<ID3D12Resource> m_texture;
    XMUINT2 m_size{};
    size_t m_descHeapIdx{ 0 };
    std::wstring m_filename{};
    Rectangle m_rect{};
    bool m_fullSize{ false };
};

XMUINT2 Texture::GetSize() const noexcept { return { static_cast<uint32_t>(m_rect.width), static_cast<uint32_t>(m_rect.height) }; }
const std::wstring& Texture::GetFilename() const noexcept { return m_filename; }
const Rectangle& Texture::GetRect() const noexcept { return m_rect; }
bool Texture::IsFullSize() const noexcept { return m_fullSize; }

