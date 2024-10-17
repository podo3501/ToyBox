#pragma once
#include "../Include/IRenderer.h"

class CFont
{
public:
    CFont();
    ~CFont();

    bool Load(ID3D12Device* device, ResourceUploadBatch* upload, const DescriptorHeap* descHeap,
        const wstring& filename, size_t descHeapIdx);

    inline const wstring& GetFilename() const noexcept { return m_filename; }
    inline float GetLineSpacing() const noexcept { return m_font->GetLineSpacing(); }
    Rectangle MeasureText(const wstring& text, const Vector2& position) const;
    void DrawString(SpriteBatch* spriteBatch, const wstring& text, const Vector2 pos, const FXMVECTOR& color) const;

private:
    unique_ptr<SpriteFont> m_font;
    wstring m_filename;
};
