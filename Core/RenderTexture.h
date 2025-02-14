#pragma once

struct IComponent;

class RenderTexture
{
	const static int RenderTargetCount = 3;

public:
	RenderTexture(ID3D12Device* device, DescriptorHeap* srvDescriptor);
	~RenderTexture();

	bool Create(DXGI_FORMAT texFormat, XMUINT2 size, size_t offset, IComponent* component);
	bool ModifyRenderTexture(const XMUINT2& size);
	void Render(ID3D12GraphicsCommandList* commandList, IRender* renderer, SpriteBatch* sprite);
	size_t GetIndex() const noexcept { return m_offset; }
	ImTextureID GetTextureID() const noexcept;
	void Reset() {}
	inline wstring GetFilename() const noexcept { return L""; }

private:
	inline D3D12_CLEAR_VALUE GetClearValue() const noexcept;
	inline D3D12_RESOURCE_DESC GetResourceDesc(DXGI_FORMAT format, const XMUINT2& size) const noexcept;
	inline Microsoft::WRL::ComPtr<ID3D12Resource>& GetTextureResource() noexcept;
	void CreateRtvAndSrv(ID3D12Resource* resource);

	ID3D12Device* m_device;
	DescriptorHeap* m_srvDescriptor;
	unique_ptr<DescriptorHeap> m_rtvDescriptor;

	int m_renderTargetIndex{ 0 };
	Microsoft::WRL::ComPtr<ID3D12Resource> m_renderTargetTexture[RenderTargetCount];

	IComponent* m_component;
	size_t m_offset{};
	D3D12_RESOURCE_DESC m_resDesc{};
};
