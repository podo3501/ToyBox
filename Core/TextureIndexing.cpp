#include "pch.h"
#include "TextureIndexing.h"
#include "DeviceResources.h"
#include "Texture.h"
#include "Font.h"
#include "Utility.h"

TextureIndexing::~TextureIndexing() = default;
TextureIndexing::TextureIndexing(DX::DeviceResources* deviceRes, DescriptorHeap* descHeap, ResourceUploadBatch* upload, SpriteBatch* sprite) :
    m_deviceResources{ deviceRes },
    m_descHeap{ descHeap }, 
    m_upload{ upload }, 
    m_sprite{ sprite }
{}

bool TextureIndexing::IsExist(const wstring& filename, const Rectangle* rect, size_t& outIndex, XMUINT2* outSize)
{
    //���� ������ �ִ��� Ȯ���Ѵ�.
    vector<unique_ptr<Texture>>::iterator find = m_textures.end();
    if (rect == nullptr)     //�̹��� ��ü�� ����Ѵٸ�
    {
        find = ranges::find_if(m_textures, [&filename](const auto& tex) {
            return (tex->GetFilename() == filename && tex->IsFullSize());
            });
    }
    else
    {
        find = ranges::find_if(m_textures, [&filename, rect](const auto& tex) {
            return (tex->GetFilename() == filename && tex->GetRect() == *rect);
            });
    }

    if (find != m_textures.end())
    {
        outIndex = distance(m_textures.begin(), find);
        if (outSize) (*outSize) = (*find)->GetSize();
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
    if (!font->Load(device, m_upload, m_descHeap, filename, m_resourceFilenames.size())) return false;

    outIndex = m_fonts.size();    //�ε����� ������Ų��.
    m_fonts.emplace_back(move(font));
    m_resourceFilenames.emplace_back(filename); //�� ������ heapindex�� �����Ѵ�.
    
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

bool TextureIndexing::LoadTexture(const wstring& filename, const Rectangle* rect, size_t& outIndex, XMUINT2* outSize)
{
    if (IsExist(filename, rect, outIndex, outSize))
        return true;

    auto findFilename = ranges::find(m_resourceFilenames, filename);

    unique_ptr<Texture> tex = nullptr;
    if (findFilename == m_resourceFilenames.end())
    {
        tex = make_unique<Texture>();
        auto device = m_deviceResources->GetD3DDevice();
        tex->Upload(device, m_descHeap, m_upload, filename, rect, m_resourceFilenames.size());
    }
    else //���� �ؽ��縦 �ε��Ϸ� �Ѵٸ�
    {
        auto findSameTex = ranges::find_if(m_textures, [&filename](const auto& tex) {
            return (tex->GetFilename() == filename);
            });
        tex = make_unique<Texture>(findSameTex->get(), rect);
    }

    outIndex = m_textures.size();    //�ε����� ������Ų��.
    if (outSize) (*outSize) = tex->GetSize();
    m_textures.emplace_back(move(tex));
    m_resourceFilenames.emplace_back(filename);

    return true;
}

bool TextureIndexing::GetTextureAreaList(const wstring& filename, const UINT32& bgColor, vector<Rectangle>& outList)
{
    auto find = ranges::find_if(m_textures, [&filename](const auto& tex) {
        return (tex->GetFilename() == filename);
        });
    if (find == m_textures.end()) return false;

    ReturnIfFalse((*find)->GetTextureAreaList(m_deviceResources, bgColor, outList));

    return true;
}

void TextureIndexing::Render(size_t index, const RECT& dest, const RECT* source)
{
    if (m_textures.empty() || index > m_textures.size() - 1) 
        return;
    m_textures[index]->Draw(m_sprite, m_descHeap, dest, source);
}

//�ؽ��Ŀ� ������ �� ��쿡�� SrvOffset������ SrvDescriptor���� �������� �ؽ��縦 ������ �� �ִ�.

void TextureIndexing::Reset()
{
    ranges::for_each(m_textures, [](auto& tex) {
        tex->Reset();
        });
}

D3D12_GPU_DESCRIPTOR_HANDLE TextureIndexing::GetSrvHandle()
{
    return m_descHeap->GetGpuHandle(0);
}
