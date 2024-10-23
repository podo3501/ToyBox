#pragma once

struct IRenderItem;

class RenderTexture
{
public:
	RenderTexture(ID3D12Device* device, DescriptorHeap* srvDescriptor);
	~RenderTexture();

	bool Create(DXGI_FORMAT texFormat, XMUINT2 size, size_t offset, IRenderItem* renderItem);
	void Render(ID3D12GraphicsCommandList* commandList, IRender* renderer, SpriteBatch* sprite, IRenderItem* renderItem);
	ImTextureID GetTextureID() const noexcept { return static_cast<ImTextureID>(m_srvHandle.ptr); }

private:
	ID3D12Device* m_device;
	DescriptorHeap* m_srvDescriptor;
	unique_ptr<DescriptorHeap> m_rtvDescriptor;
	Microsoft::WRL::ComPtr<ID3D12Resource> m_renderTargetTexture;

	D3D12_GPU_DESCRIPTOR_HANDLE m_srvHandle;
	IRenderItem* m_renderItem;
};
