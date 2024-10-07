#pragma once
#include "../Include/IRenderer.h"

class Texture;

class TextureIndexing : public ILoadData, public IRender
{
public:
    TextureIndexing(ID3D12Device* device, DescriptorHeap* descHeap, ResourceUploadBatch* upload, SpriteBatch* sprite);
    ~TextureIndexing();

    //ILoadData
    virtual bool LoadTexture(const wstring& filename, const Rectangle* rect, size_t& outIndex, XMUINT2* outSize) override;

    //IRender
    virtual void Render(size_t index, const RECT& dest, const RECT* source) override;

    void Reset();

private:
	bool IsExist(const wstring& filename, const Rectangle* rect, std::size_t& outIndex, DirectX::XMUINT2* outSize);

    ID3D12Device* m_device{ nullptr };
    DescriptorHeap* m_descHeap{ nullptr };
    ResourceUploadBatch* m_upload{ nullptr };
    SpriteBatch* m_sprite{ nullptr };
	vector<wstring> m_texFilenames;
	map<size_t, unique_ptr<Texture>> m_textures;
};
