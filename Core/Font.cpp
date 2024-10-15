#include "pch.h"
#include "Font.h"

CFont::CFont() = default;
CFont::~CFont() = default;

bool CFont::Load(ID3D12Device* device, ResourceUploadBatch* upload, const DescriptorHeap* descHeap,
    const wstring& filename, size_t descHeapIdx)
{
    m_font = make_unique<SpriteFont>(device, *upload, filename.c_str(),
        descHeap->GetCpuHandle(descHeapIdx),
        descHeap->GetGpuHandle(descHeapIdx));
    m_filename = filename;

    return true;
}

void CFont::GetSize()
{
}