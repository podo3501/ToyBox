#pragma once
#include "../Include/IRenderer.h"

class CFont
{
public:
    CFont();
    CFont(ID3D12Device* device, DescriptorPile* descPile) noexcept;
    ~CFont();

    bool Load(ResourceUploadBatch* upload, const wstring& filename, size_t index);
    bool Load(ID3D12Device* device, ResourceUploadBatch* upload, const DescriptorHeap* descHeap,
        const wstring& filename, size_t descHeapIdx);

    inline size_t GetIndex() const noexcept { return *m_index; }
    inline const wstring& GetFilename() const noexcept { return m_filename; }
    inline float GetLineSpacing() const noexcept { return m_font->GetLineSpacing(); }
    Rectangle MeasureText(const wstring& text, const Vector2& position) const;
    void DrawString(SpriteBatch* spriteBatch, const wstring& text, const Vector2 pos, const FXMVECTOR& color) const;

private:
    ID3D12Device* m_device;
    DescriptorPile* m_descPile;
    unique_ptr<SpriteFont> m_font;
    wstring m_filename;
    optional<size_t> m_index;
};
