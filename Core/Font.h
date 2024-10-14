#pragma once

class CFont
{
public:
    CFont();
    ~CFont();

    bool Load(ID3D12Device* device, ResourceUploadBatch* upload, const DescriptorHeap* descHeap,
        const wstring& filename, size_t descHeapIdx);

    inline const wstring& GetFilename() const noexcept { return m_filename; }

private:
    unique_ptr<SpriteFont> m_font;
    wstring m_filename;
};
