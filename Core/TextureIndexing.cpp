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

template <typename Container>
auto FindResourceByFilename(const Container& list, const wstring& filename)
{
    auto find = ranges::find_if(list, [&filename](const auto& tex) {
        return tex != nullptr && tex->GetFilename() == filename;
        });

    if (find != list.end())
        return find->get();

    return static_cast<decltype(find->get())>(nullptr); // nullptr을 컨테이너에 들어있는 타입으로 변환
}

bool TextureIndexing::LoadFont(const wstring& filename, size_t& outIndex)
{
    if (auto find = FindResourceByFilename(m_fontList, filename); find)
    {
        outIndex = find->GetIndex();
        return true;
    }

    unique_ptr<CFont> font = make_unique<CFont>(m_device, m_descHeapPile);
    auto offset = m_descHeapPile->Allocate();
    ReturnIfFalse(font->Load(m_upload, filename, offset));

    outIndex = font->GetIndex();
    m_fontList[offset] = move(font);

    return true;
}

void TextureIndexing::DrawString(size_t index, const wstring& text, const Vector2& pos, const FXMVECTOR& color) const
{
    m_fontList[index]->DrawString(m_sprite, text, pos, color);
}

Rectangle TextureIndexing::MeasureText(size_t index, const wstring& text, const Vector2& position)
{
    return m_fontList[index]->MeasureText(text, position);
}

float TextureIndexing::GetLineSpacing(size_t index) const noexcept
{
    return m_fontList[index]->GetLineSpacing();
}

//size_t TextureIndexing::GenerateSrvOffset(TextureType textureType) noexcept
//{
//    //비어있는 인덱스를 하나 꺼내 준다.
//    size_t curIdx = m_descHeapPile->Allocate();
//    SrvOffsetAt(textureType, curIdx) = curIdx;
//    return curIdx;
//}

bool TextureIndexing::LoadTexture(const wstring& filename, size_t& outIndex, XMUINT2* outSize)
{
    if (auto find = FindResourceByFilename(m_textureList, filename); find)
    {
        outIndex = find->GetIndex();
        if (outSize) *outSize = find->GetSize();
        return true;
    }

    auto tex = make_unique<Texture>(m_device, m_descHeapPile);
    auto offset = m_descHeapPile->Allocate();
    tex->Upload(m_upload, filename, offset);

    outIndex = tex->GetIndex();
    if (outSize) (*outSize) = tex->GetSize();
    m_textureList[offset] = move(tex);

    //m_renderList[offset].SetRenderFunction([ptr = tex.get()]() {
    //    RECT dest = {};
    //    RECT* source = nullptr;
    //    SpriteBatch* spriteBatch = nullptr;
    //    ptr->Draw(spriteBatch, dest, source);
    //    });
    
    return true;
}

bool TextureIndexing::CreateRenderTexture(const XMUINT2& size, IComponent* component, size_t& outIndex, ImTextureID* outTextureID)
{
    unique_ptr<RenderTexture> renderTexture = make_unique<RenderTexture>(m_device, m_descHeapPile);

    auto format = m_deviceResources->GetBackBufferFormat();
    //ReturnIfFalse(renderTexture->Create(format, size, m_renderTexOffset->Increase(), component));
    //outTextureID = renderTexture->GetTextureID();

    return true;
}

optional<vector<Rectangle>> TextureIndexing::GetTextureAreaList(const wstring& filename, const UINT32& bgColor)
{
    auto find = FindResourceByFilename(m_textureList, filename);
    if (!find) return nullopt;

    vector<Rectangle> areaList;
    if(!find->GetTextureAreaList(m_deviceResources, bgColor, areaList)) return nullopt;

    return areaList;
}

void TextureIndexing::Render(size_t index, const RECT& dest, const RECT* source)
{
    if (m_textureList.empty() || index > m_textureList.size() - 1) return;
    if (!m_textureList[index]) return;

    m_textureList[index]->Draw(m_sprite, dest, source);
}

void TextureIndexing::Reset()
{
    ranges::for_each(m_textureList, [](auto& tex) {
        if(tex)
            tex->Reset();
        });
}

D3D12_GPU_DESCRIPTOR_HANDLE TextureIndexing::GetSrvHandle()
{
    return m_descHeapPile->GetGpuHandle(0);
}
