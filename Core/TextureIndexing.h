#pragma once
#include "../Include/IRenderer.h"
#include "Setting.h"

class Texture;
class CFont;
class RenderTexture;

namespace DX
{
    class DeviceResources;
}

//variant�� �� ������ ��׵��� �������� �ʴµ�, ���߿� ������� �ذ�ȴٸ� ������� �ϴ� ���� ���� �� �� ���ð� ����.
using SrvResource = variant<unique_ptr<Texture>, unique_ptr<CFont>, unique_ptr<RenderTexture>>;

class TextureIndexing : public ILoadData, public IGetValue, public IRender
{
public:
    TextureIndexing(DX::DeviceResources* deviceRes,
        DescriptorPile* descriptorPile,
        ResourceUploadBatch* upload, 
        SpriteBatch* sprite); //SpriteBatch�� ���⼭ ����� �� �� �ִ�.
    ~TextureIndexing();

    //ILoadData
    virtual bool LoadTexture(const wstring& filename, size_t& outIndex, XMUINT2* outSize) override;
    virtual bool LoadFont(const wstring& filename, size_t& outIndex) override;

    //IGetValue
    virtual bool CreateRenderTexture(const XMUINT2& size, IComponent* component, size_t& outIndex, UINT64* outGfxMemOffset) override;
    virtual Rectangle MeasureText(size_t index, const wstring& text, const Vector2& position) override;
    virtual float GetLineSpacing(size_t index) const noexcept override;
    virtual optional<vector<Rectangle>> GetTextureAreaList(const wstring& filename, const UINT32& bgColor) override;
    virtual void ReleaseTexture(size_t idx) noexcept override;
    virtual bool ModifyRenderTexture(size_t index, const XMUINT2& size) override;

    //IRender
    virtual void Render(size_t index, const RECT& dest, const RECT* source) override;
    virtual void DrawString(size_t index, const wstring& text, const Vector2& pos, const FXMVECTOR& color) const override;

    void DrawRenderTextures();
    void Reset();

private:
    void ReleaseDescriptor(size_t idx) noexcept;
    size_t AllocateDescriptor() noexcept;

    inline Texture* GetTexture(size_t index) const { return get<unique_ptr<Texture>>(m_srvResources[index]).get(); }
    inline CFont* GetFont(size_t index) const { return get<unique_ptr<CFont>>(m_srvResources[index]).get(); }
    inline RenderTexture* GetRenderTex(size_t index) const { return get<unique_ptr<RenderTexture>>(m_srvResources[index]).get(); }

    DX::DeviceResources* m_deviceResources;
    ID3D12Device* m_device;
    DescriptorPile* m_descHeapPile;
    ResourceUploadBatch* m_upload;
    SpriteBatch* m_sprite;

    array<int, MAX_DESC> m_refCount{};
    array<SrvResource, MAX_DESC> m_srvResources;
    vector<size_t> m_freeDescIndices; //�����ǰ��� ��Ȱ��� �ε��� ����
    size_t m_nextDescIdx{ 0 };  //���Ӱ� �Ҵ��� �ε���
};