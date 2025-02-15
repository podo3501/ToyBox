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

static SrvResource* FindResourceByFilename(array<SrvResource, MAX_DESC>& resources, const wstring& filename)
{
    auto find = ranges::find_if(resources, [&filename](const SrvResource& res) {
        return visit([&filename](const auto& ptr) {
            return ptr && ptr->GetFilename() == filename;
            }, res);
        });

    if (find != resources.end()) {
        return &(*find);
    }

    return nullptr;
}
//LoadFont와 LoadTexture를 통합하고 싶은데 variant로 되어 있으면 코드가 너무 어렵게 된다. 그래서 클래스가 variant에서 상속으로 바꾼후 리팩토링 예정
bool TextureIndexing::LoadFont(const wstring& filename, size_t& outIndex)
{
    if (auto find = FindResourceByFilename(m_srvResources, filename); find)
    {
        CFont* curFont = get<unique_ptr<CFont>>(*find).get();
        outIndex = curFont->GetIndex();
        m_refCount[outIndex]++;
        return true;
    }

    unique_ptr<CFont> font = make_unique<CFont>(m_device, m_descHeapPile);
    auto offset = AllocateDescriptor();
    ReturnIfFalse(font->Load(m_upload, filename, offset));

    outIndex = font->GetIndex();
    m_srvResources[offset] = move(font);

    return true;
}

bool TextureIndexing::LoadTexture(const wstring& filename, size_t& outIndex, XMUINT2* outSize)
{
    if (auto find = FindResourceByFilename(m_srvResources, filename); find)
    {
        Texture* curTex = get<unique_ptr<Texture>>(*find).get();
        outIndex = curTex->GetIndex();
        m_refCount[outIndex]++;
        if (outSize) *outSize = curTex->GetSize();
        return true;
    }

    auto tex = make_unique<Texture>(m_device, m_descHeapPile);
    auto offset = AllocateDescriptor();
    tex->Upload(m_upload, filename, offset);

    outIndex = tex->GetIndex();
    if (outSize) *outSize = tex->GetSize();
    m_srvResources[offset] = move(tex);
    
    return true;
}

bool TextureIndexing::CreateRenderTexture(const XMUINT2& size, IComponent* component, size_t& outIndex, UINT64* outGfxMemOffset)
{
    auto renderTex = make_unique<RenderTexture>(m_device, m_descHeapPile);

    auto format = m_deviceResources->GetBackBufferFormat();
    auto offset = AllocateDescriptor();
    if (!renderTex->Create(format, size, offset, component))
    {
        ReleaseDescriptor(offset);
        return false;
    }
    outIndex = renderTex->GetIndex();
    if(outGfxMemOffset) *outGfxMemOffset = renderTex->GetGraphicMemoryOffset();
    m_srvResources[offset] = move(renderTex);

    return true;
}

bool TextureIndexing::ModifyRenderTexture(size_t index, const XMUINT2& size)
{
    return GetRenderTex(index)->ModifyRenderTexture(size);
}

void TextureIndexing::DrawString(size_t index, const wstring& text, const Vector2& pos, const FXMVECTOR& color) const 
{
    GetFont(index)->DrawString(m_sprite, text, pos, color);
}

Rectangle TextureIndexing::MeasureText(size_t index, const wstring& text, const Vector2& position)
{
    return GetFont(index)->MeasureText(text, position);
}

float TextureIndexing::GetLineSpacing(size_t index) const noexcept
{
    return GetFont(index)->GetLineSpacing();
}

optional<vector<Rectangle>> TextureIndexing::GetTextureAreaList(const wstring& filename, const UINT32& bgColor)
{
    auto find = FindResourceByFilename(m_srvResources, filename);
    if (!find) return nullopt;

    Texture* curTex = get<unique_ptr<Texture>>(*find).get();
    vector<Rectangle> areaList;
    if (!curTex->GetTextureAreaList(m_deviceResources, bgColor, areaList)) return nullopt;

    return areaList;
}

void TextureIndexing::Render(size_t index, const RECT& dest, const RECT* source)
{
    assert(!m_srvResources.empty() && index <= m_srvResources.size() - 1); //assert로 한 이유는 release일때는 조금이라도 동작을 안하게 하기 위해서이다.
    assert(get_if<unique_ptr<Texture>>(&m_srvResources[index]));

    GetTexture(index)->Draw(m_sprite, dest, source);
}

void TextureIndexing::DrawRenderTextures()
{
    auto views = m_srvResources
        | views::filter([](const SrvResource& resource) {
        return holds_alternative<unique_ptr<RenderTexture>>(resource) &&
            get<unique_ptr<RenderTexture>>(resource).get() != nullptr;
            })
        | views::transform([](const SrvResource& resource) -> RenderTexture* {
        return get<unique_ptr<RenderTexture>>(resource).get();
            });

    auto commandList = m_deviceResources->GetCommandList();
    for (auto renderTex : views)
        renderTex->Render(commandList, this, m_sprite);
}

void TextureIndexing::ReleaseTexture(size_t idx) noexcept
{
    if (idx < 0 || idx >= MAX_DESC) return;

    if (m_refCount[idx] > 0 )
    {
        m_refCount[idx]--;
        return;
    }
        
    m_deviceResources->WaitForGpu();
    std::visit([](auto& resource) {
        resource = nullptr;
        }, m_srvResources[idx]);
    ReleaseDescriptor(idx);
}

void TextureIndexing::Reset()
{
    ranges::for_each(m_srvResources, [](auto& res) {
        visit([](const auto& ptr) {
            ptr->Reset();
            }, res);
        });
}

void TextureIndexing::ReleaseDescriptor(size_t idx) noexcept
{
    assert(idx < MAX_DESC);
    
    m_freeDescIndices.push_back(idx);
}

size_t TextureIndexing::AllocateDescriptor() noexcept
{
    if (!m_freeDescIndices.empty()) 
    {
        size_t idx = m_freeDescIndices.back();
        m_freeDescIndices.pop_back();
        return idx;
    }

    assert(m_nextDescIdx < MAX_DESC);
    return m_nextDescIdx++;
}
