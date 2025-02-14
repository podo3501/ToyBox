#pragma once
#include "../Include/IRenderer.h"
#include "Setting.h"

class Texture;
class CFont;

namespace DX
{
    class DeviceResources;
}

// ���� ���ڸ� �޴� `std::function`�� ������ ����ü
struct RenderSlot 
{
    template <typename... ParamTypes>
    void SetFunc(ParamTypes&&... params)
    {

    }

};

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
    virtual bool CreateRenderTexture(const XMUINT2& size, IComponent* component, size_t& outIndex, ImTextureID* outTextureID) override;
    virtual bool LoadFont(const wstring& filename, size_t& outIndex) override;

    //IGetValue
    virtual Rectangle MeasureText(size_t index, const wstring& text, const Vector2& position) override;
    virtual float GetLineSpacing(size_t index) const noexcept override;
    virtual optional<vector<Rectangle>> GetTextureAreaList(const wstring& filename, const UINT32& bgColor) override;

    //IRender
    virtual void Render(size_t index, const RECT& dest, const RECT* source) override;
    virtual void DrawString(size_t index, const wstring& text, const Vector2& pos, const FXMVECTOR& color) const override;

    void Reset();
    D3D12_GPU_DESCRIPTOR_HANDLE GetSrvHandle();

private:
    enum class TextureType
    {
        Texture,
        RenderTexture
    };

    DX::DeviceResources* m_deviceResources;
    ID3D12Device* m_device;
    DescriptorPile* m_descHeapPile;
    ResourceUploadBatch* m_upload;
    SpriteBatch* m_sprite;

    array<unique_ptr<Texture>, SrvCount> m_textureList;
    array<unique_ptr<CFont>, SrvCount> m_fontList;
    array<RenderSlot, SrvCount> m_renderList;

    //constexpr static int TextureTypeCount = 2;
    //constexpr static int SrvOffsetCount = 100;
    //array<unique_ptr<Texture>, SrvOffsetCount * TextureTypeCount> m_textures;

    //�ؽ����� Ÿ�԰� �ε����� ������ ���� ����ִ� srvOffset���� �����ش�.
    //SrvOffset���� ������ �����ؼ� �� ��쿡�� �ؽ����� ������ ������ ��� �������ؾ� �ϰ�, ���� ���� �������� �����.
    //inline optional<size_t>& SrvOffsetAt(TextureType textureType, size_t srvIdx) { return m_srvOffsetTable[static_cast<int>(textureType) * SrvOffsetCount + srvIdx]; } //2���� �迭�� �ƴ� 1���� �迭�� ���� ������ ĳ�� ���߷��� ���̱� ���ؼ��̴�.
    //size_t GenerateSrvOffset(TextureType textureType) noexcept;
    //array<optional<size_t>, SrvOffsetCount* TextureTypeCount> m_srvOffsetTable;
};