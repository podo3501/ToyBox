#include "pch.h"
#include "TextureIndexing.h"
#include "Texture.h"

TextureIndexing::~TextureIndexing() = default;
TextureIndexing::TextureIndexing(ID3D12Device* device, DescriptorHeap* descHeap, ResourceUploadBatch* upload, SpriteBatch* sprite) :
    m_device{ device }, m_descHeap{ descHeap }, m_upload{ upload }, m_sprite{ sprite }
{}

bool TextureIndexing::IsExist(const wstring& filename, const Rectangle* rect, size_t& outIndex, XMUINT2* outSize)
{
    //같은 파일이 있는지 확인한다.
    map<size_t, unique_ptr<Texture>>::iterator find = m_textures.end();
    if (rect == nullptr)     //이미지 전체를 사용한다면
    {
        find = ranges::find_if(m_textures, [&filename](const auto& tex) {
            return (tex.second->GetFilename() == filename && tex.second->IsFullSize());
            });
    }
    else
    {
        find = ranges::find_if(m_textures, [&filename, rect](const auto& tex) {
            return (tex.second->GetFilename() == filename && tex.second->GetRect() == *rect);
            });
    }

    if (find != m_textures.end())
    {
        outIndex = find->first;
        if (outSize) (*outSize) = find->second->GetSize();
        return true;
    }

    return false;
}

bool TextureIndexing::LoadFont(const wstring& filename, size_t& outIndex)
{
    std::size_t descHeapIdx{ 0 };
    std::unique_ptr<SpriteFont> font = make_unique<SpriteFont>(m_device, *m_upload, filename.c_str(),
        m_descHeap->GetCpuHandle(descHeapIdx),
        m_descHeap->GetGpuHandle(descHeapIdx));

    return true;
}

bool TextureIndexing::LoadTexture(const wstring& filename, const Rectangle* rect, size_t& outIndex, XMUINT2* outSize)
{
    if (IsExist(filename, rect, outIndex, outSize))
        return true;

    auto findFilename = ranges::find(m_texFilenames, filename);

    unique_ptr<Texture> tex = nullptr;
    if (findFilename == m_texFilenames.end())
    {
        tex = make_unique<Texture>(m_device, m_descHeap);
        tex->Upload(m_upload, filename, rect, m_texFilenames.size());
    }
    else //같은 텍스춰를 로딩하려 한다면
    {
        auto findSameTex = ranges::find_if(m_textures, [&filename](const auto& tex) {
            return (tex.second->GetFilename() == filename);
            });
        tex = make_unique<Texture>(findSameTex->second.get(), rect);
    }

    size_t newIndex = m_textures.size();    //인덱스를 증가시켜서 texture를 만든다.
    outIndex = newIndex;
    if (outSize) (*outSize) = tex->GetSize();
    m_textures.insert(make_pair(newIndex, move(tex)));
    m_texFilenames.emplace_back(filename);

    return true;
}

void TextureIndexing::Render(size_t index, const RECT& dest, const RECT* source)
{
    m_textures[index]->Draw(m_sprite, dest, source);
}

void TextureIndexing::Reset()
{
    ranges::for_each(m_textures | views::values, [](auto& tex) {
        tex->Reset();
        });
}
