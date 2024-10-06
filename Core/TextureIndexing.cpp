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

bool TextureIndexing::LoadTexture(const wstring& filename, const Rectangle* rect, size_t& outIndex, XMUINT2* outSize)
{
    if (IsExist(filename, rect, outIndex, outSize))
        return true;

    auto findFilename = ranges::find(m_texFilenames, filename);
    unique_ptr<Texture> tex = make_unique<Texture>(m_device, m_descHeap);

    //텍스춰는 로딩한 적이 있다면
    size_t descHeapIndex{ 0 };
    if (findFilename == m_texFilenames.end())
    {
        descHeapIndex = m_texFilenames.size();
        tex->Upload(m_upload, filename, rect, descHeapIndex);
    }
    else
    {
        auto findSameTex = ranges::find_if(m_textures, [&filename](const auto& tex) {
            return (tex.second->GetFilename() == filename);
            });
        Texture* curTex = findSameTex->second.get();
        tex->Set(curTex->GetTextureResource(), filename, rect, curTex->GetDescHeapIndex());
    }

    size_t newIndex = m_textures.size();    //인덱스를 증가시켜서 texture를 만든다.
    outIndex = newIndex;
    if (outSize) (*outSize) = tex->GetSize();
    m_textures.insert(make_pair(newIndex, move(tex)));
    m_texFilenames.emplace_back(filename);

    return true;
}

void TextureIndexing::Render(size_t index, const XMUINT2& size, const Vector2& position, const XMFLOAT2& origin)
{
    m_textures[index]->Draw(m_sprite, size, position, origin);
}

void TextureIndexing::Reset()
{
    ranges::for_each(m_textures | views::values, [](auto& tex) {
        tex->Reset();
        });
}
