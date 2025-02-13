#pragma once
#include "../Include/IRenderer.h"
#include "Setting.h"

class Texture;
class CFont;

namespace DX
{
    class DeviceResources;
}

class TextureIndexing : public ILoadData, public IGetValue, public IRender
{
public:
    TextureIndexing(DX::DeviceResources* deviceRes,
        DescriptorPile* descriptorPile,
        ResourceUploadBatch* upload, 
        SpriteBatch* sprite); //SpriteBatch는 여기서 만들어 줄 수 있다.
    ~TextureIndexing();

    //ILoadData
    virtual bool LoadTexture(const wstring& filename, const Rectangle* rect, size_t& outIndex, XMUINT2* outSize) override;
    virtual bool LoadTexture(const wstring& filename, size_t& outIndex, XMUINT2* outSize) override;
    virtual bool CreateRenderTexture(const XMUINT2& size, IComponent* component, size_t& outIndex, ImTextureID* outTextureID) override;
    virtual bool LoadFont(const wstring& filename, size_t& outIndex) override;

    //IGetValue
    virtual Rectangle MeasureText(size_t index, const wstring& text, const Vector2& position) override;
    virtual float GetLineSpacing(size_t index) const noexcept override;

    virtual bool GetTextureAreaList(const wstring& filename, const UINT32& bgColor, vector<Rectangle>& outList) override;

    //IRender
    virtual void Render(size_t index, const RECT& dest, const RECT* source) override;
    virtual void DrawString(size_t index, const wstring& text, const Vector2& pos, const FXMVECTOR& color) const override;

    void Reset();
    D3D12_GPU_DESCRIPTOR_HANDLE GetSrvHandle();

private:
	bool IsExist(const wstring& filename, const Rectangle* rect, std::size_t& outIndex, DirectX::XMUINT2* outSize);

    DX::DeviceResources* m_deviceResources;
    ID3D12Device* m_device;
    DescriptorPile* m_descHeapPile;
    ResourceUploadBatch* m_upload;
    SpriteBatch* m_sprite;
	vector<wstring> m_resourceFilenames;

    enum class TextureType
    {
        Texture,
        RenderTexture
    };
    constexpr static int TextureTypeCount = 2;
    constexpr static int SrvOffsetCount = 100;
    

	vector<unique_ptr<Texture>> m_textures;
    //array<unique_ptr<Texture>, SrvOffsetCount * TextureTypeCount> m_textures;
    vector<unique_ptr<CFont>> m_fonts;
    array<unique_ptr<Texture>, SrvCount> m_textureList;

    //텍스춰의 타입과 인덱스를 넣으면 지금 비어있는 srvOffset값을 돌려준다.
    //SrvOffset값을 구간을 지정해서 할 경우에는 텍스춰의 갯수가 많아질 경우 재조정해야 하고, 적을 경우는 공간낭비가 생긴다.
    inline optional<size_t>& SrvOffsetAt(TextureType textureType, size_t srvIdx) { return m_srvOffsetTable[static_cast<int>(textureType) * SrvOffsetCount + srvIdx]; } //2차원 배열이 아닌 1차원 배열을 쓰는 이유는 캐쉬 적중률을 높이기 위해서이다.
    size_t GenerateSrvOffset(TextureType textureType) noexcept;
    array<optional<size_t>, SrvOffsetCount* TextureTypeCount> m_srvOffsetTable;
};