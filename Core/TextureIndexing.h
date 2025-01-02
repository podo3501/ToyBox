#pragma once
#include "../Include/IRenderer.h"

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
        DescriptorHeap* descHeap, 
        ResourceUploadBatch* upload, 
        SpriteBatch* sprite); //SpriteBatch는 여기서 만들어 줄 수 있다.
    ~TextureIndexing();

    //ILoadData
    virtual bool LoadTexture(const wstring& filename, const Rectangle* rect, size_t& outIndex, XMUINT2* outSize) override;
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
    DescriptorHeap* m_descHeap{ nullptr };
    ResourceUploadBatch* m_upload{ nullptr };
    SpriteBatch* m_sprite{ nullptr };
	vector<wstring> m_resourceFilenames;

	vector<unique_ptr<Texture>> m_textures;
    vector<unique_ptr<CFont>> m_fonts;
};