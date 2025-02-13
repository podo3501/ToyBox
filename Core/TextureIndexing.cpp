#include "pch.h"
#include "TextureIndexing.h"
#include "DeviceResources.h"
#include "Texture.h"
#include "RenderTexture.h"
#include "Font.h"
#include "Utility.h"

TextureIndexing::~TextureIndexing() = default;
TextureIndexing::TextureIndexing(DX::DeviceResources* deviceRes, DescriptorPile* descriptorPile, ResourceUploadBatch* upload, SpriteBatch* sprite) :
    m_deviceResources{ deviceRes },
    m_device{ deviceRes->GetD3DDevice() },
    m_descHeapPile{ descriptorPile },
    m_upload{ upload }, 
    m_sprite{ sprite }
{}

bool TextureIndexing::IsExist(const wstring& filename, const Rectangle* rect, size_t& outIndex, XMUINT2* outSize)
{
    auto find = ranges::find_if(m_textures, [&filename, rect](const auto& tex) {
        return tex->GetFilename() == filename && (rect ? tex->GetRect() == *rect : tex->IsFullSize());
        });

    //auto find = ranges::find_if(m_textures, [&filename, rect](const auto& tex) {
    //    return tex->GetFilename() == filename;
    //    });

    if (find != m_textures.end())
    {
        outIndex = distance(m_textures.begin(), find);
        if (outSize) *outSize = (*find)->GetSize();
        return true;
    }

    return false;
}

bool TextureIndexing::LoadFont(const wstring& filename, size_t& outIndex)
{
    auto find = ranges::find(m_resourceFilenames, filename);
    if (find != m_resourceFilenames.end())
    {
        auto findFont = ranges::find_if(m_fonts, [&filename](const auto& font) {
            return (font->GetFilename() == filename);
            });
        outIndex = distance(m_fonts.begin(), findFont);
        return true;
    }

    unique_ptr<CFont> font = make_unique<CFont>();
    auto device = m_deviceResources->GetD3DDevice();
    if (!font->Load(device, m_upload, m_descHeapPile, filename, m_resourceFilenames.size())) return false;

    outIndex = m_fonts.size();    //인덱스를 증가시킨다.
    m_fonts.emplace_back(move(font));
    m_resourceFilenames.emplace_back(filename); //이 변수가 heapindex를 제공한다.
    
    return true;
}

Rectangle TextureIndexing::MeasureText(size_t index, const wstring& text, const Vector2& position)
{
    return m_fonts[index]->MeasureText(text, position);
}

float TextureIndexing::GetLineSpacing(size_t index) const noexcept
{ 
    return m_fonts[index]->GetLineSpacing();
}

void TextureIndexing::DrawString(size_t index, const wstring& text, const Vector2& pos, const FXMVECTOR& color) const
{
    m_fonts[index]->DrawString(m_sprite, text, pos, color);
}

size_t TextureIndexing::GenerateSrvOffset(TextureType textureType) noexcept
{
    //비어있는 인덱스를 하나 꺼내 준다.
    size_t curIdx = m_descHeapPile->Allocate();
    SrvOffsetAt(textureType, curIdx) = curIdx;
    return curIdx;
}

//bool TextureIndexing::LoadTexture(const wstring& filename, const Rectangle* rect, size_t& outIndex, XMUINT2* outSize)
//{
//    if (IsExist(filename, rect, outIndex, outSize))
//        return true;
//
//    auto findFilename = ranges::find(m_resourceFilenames, filename);
//
//    unique_ptr<Texture> tex;
//    if (findFilename == m_resourceFilenames.end())
//    {
//        tex = make_unique<Texture>();
//        auto device = m_deviceResources->GetD3DDevice();
//        tex->Upload(device, m_descHeapPile, m_upload, filename, rect, m_resourceFilenames.size()); //여기 인덱스 이상
//    }
//    else //같은 텍스춰를 로딩하지만 사용하는 구역이 다를 경우
//    {
//        auto findSameTex = ranges::find_if(m_textures, [&filename](const auto& tex) {
//            return (tex->GetFilename() == filename);
//            });
//        tex = make_unique<Texture>(findSameTex->get(), rect);
//    }
//
//    outIndex = m_textures.size();    //인덱스를 증가시킨다. 여기 인덱스 이상
//    if (outSize) (*outSize) = tex->GetSize();
//    //size_t srvIdx = GenerateSrvOffset(TextureType::Texture);
//    //m_textures[srvIdx] = move(tex);
//    m_textures.emplace_back(move(tex));
//    m_resourceFilenames.emplace_back(filename);
//
//    return true;
//}

bool TextureIndexing::LoadTexture(const wstring& filename, size_t& outIndex, XMUINT2* outSize)
{
    auto find = ranges::find_if(m_textureList, [&filename](const auto& tex) { return tex->GetFilename() == filename; });
    if (find != m_textureList.end())
    {
        outIndex = distance(m_textureList.begin(), find);
        if (outSize) *outSize = (*find)->GetSize();
        return true;
    }

    auto tex = make_unique<Texture>();
    tex->Upload(m_device, m_descHeapPile, m_upload, filename); //여기 인덱스 이상

    ////////////////////////////////////////////////////////////////

    if (IsExist(filename, rect, outIndex, outSize))
        return true;

    auto findFilename = ranges::find(m_resourceFilenames, filename);

    unique_ptr<Texture> tex;
    if (findFilename == m_resourceFilenames.end())
    {
        tex = make_unique<Texture>();
        auto device = m_deviceResources->GetD3DDevice();
        tex->Upload(device, m_descHeapPile, m_upload, filename, rect, m_resourceFilenames.size()); //여기 인덱스 이상
    }
    else //같은 텍스춰를 로딩하지만 사용하는 구역이 다를 경우
    {
        auto findSameTex = ranges::find_if(m_textures, [&filename](const auto& tex) {
            return (tex->GetFilename() == filename);
            });
        tex = make_unique<Texture>(findSameTex->get(), rect);
    }

    outIndex = m_textures.size();    //인덱스를 증가시킨다. 여기 인덱스 이상
    if (outSize) (*outSize) = tex->GetSize();
    //size_t srvIdx = GenerateSrvOffset(TextureType::Texture);
    //m_textures[srvIdx] = move(tex);
    m_textures.emplace_back(move(tex));
    m_resourceFilenames.emplace_back(filename);

    return true;
}

bool TextureIndexing::CreateRenderTexture(const XMUINT2& size, IComponent* component, size_t& outIndex, ImTextureID* outTextureID)
{
    auto device = m_deviceResources->GetD3DDevice();
    unique_ptr<RenderTexture> renderTexture = make_unique<RenderTexture>(device, m_descHeapPile);

    auto format = m_deviceResources->GetBackBufferFormat();
    //ReturnIfFalse(renderTexture->Create(format, size, m_renderTexOffset->Increase(), component));
    //outTextureID = renderTexture->GetTextureID();

    return true;
}

bool TextureIndexing::GetTextureAreaList(const wstring& filename, const UINT32& bgColor, vector<Rectangle>& outList)
{
    auto find = ranges::find_if(m_textures, [&filename](const auto& tex) {
        return (tex->GetFilename() == filename);
        });

    if (find == m_textures.end()) return false;
    return ((*find)->GetTextureAreaList(m_deviceResources, bgColor, outList));
}

void TextureIndexing::Render(size_t index, const RECT& dest, const RECT* source)
{
    if (m_textures.empty() || index > m_textures.size() - 1) 
        return;
    m_textures[index]->Draw(m_sprite, m_descHeapPile, dest, source);
}

//텍스쳐에 렌더링 된 경우에는 SrvOffset값으로 SrvDescriptor에서 가져오면 텍스춰를 가져올 수 있다.

void TextureIndexing::Reset()
{
    ranges::for_each(m_textures, [](auto& tex) {
        tex->Reset();
        });
}

D3D12_GPU_DESCRIPTOR_HANDLE TextureIndexing::GetSrvHandle()
{
    return m_descHeapPile->GetGpuHandle(0);
}
