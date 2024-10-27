#pragma once

struct IRenderItem;

class RenderTexture
{
	const static int RenderTargetCount = 3;

public:
	RenderTexture(ID3D12Device* device, DescriptorHeap* srvDescriptor);
	~RenderTexture();

	bool Create(DXGI_FORMAT texFormat, XMUINT2 size, size_t offset, IRenderItem* renderItem);
	void Render(ID3D12GraphicsCommandList* commandList, IRender* renderer, SpriteBatch* sprite);
	ImTextureID GetTextureID() const noexcept { return static_cast<ImTextureID>(m_srvHandle.ptr); }

private:
	ID3D12Device* m_device;
	DescriptorHeap* m_srvDescriptor;
	unique_ptr<DescriptorHeap> m_rtvDescriptor;

	int m_renderTargetIndex{ 0 };
	Microsoft::WRL::ComPtr<ID3D12Resource> m_renderTargetTexture[RenderTargetCount];

	XMUINT2 m_size{};
	D3D12_GPU_DESCRIPTOR_HANDLE m_srvHandle{};
	IRenderItem* m_renderItem;
};
