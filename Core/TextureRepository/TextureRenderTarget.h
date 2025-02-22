#pragma once
#include "TextureResource.h"

struct ITextureRender;
struct IComponent;
namespace DX { class DeviceResources; }

class TextureRenderTarget : public TextureResource
{
public:
	static TextureResourceID GetTypeStatic() { return TextureResourceID::TextureRenderTarget; }
	virtual TextureResourceID GetTypeID() const noexcept override { return GetTypeStatic(); }

	TextureRenderTarget(DX::DeviceResources* deviceResources, DescriptorHeap* descHeap);
	~TextureRenderTarget();

	bool Create(DXGI_FORMAT texFormat, const XMUINT2& size, const XMINT2& pos, size_t offset, IComponent* component);
	bool ModifyRenderTexture(const XMUINT2& size);
	void Render(ID3D12GraphicsCommandList* commandList, ITextureRender* renderer, SpriteBatch* sprite);

private:
	bool CreateTextureResource(DXGI_FORMAT texFormat, const XMUINT2& size);
	inline D3D12_CLEAR_VALUE GetClearValue() const noexcept;
	inline D3D12_RESOURCE_DESC GetResourceDesc(DXGI_FORMAT format, const XMUINT2& size) const noexcept;
	void CreateRtvAndSrv(ID3D12Resource* resource);

	DX::DeviceResources* m_deviceResources;
	unique_ptr<DescriptorHeap> m_rtvDescriptor;
	Microsoft::WRL::ComPtr<ID3D12Resource> m_texResource;

	IComponent* m_component;
	D3D12_RESOURCE_DESC m_resDesc{};
	XMINT2 m_position{};
};
