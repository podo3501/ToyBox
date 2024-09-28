#pragma once

struct IRenderItem
{
public:
	virtual ~IRenderItem() {};
	virtual void OnDeviceLost() = 0;
	virtual void LoadResources(ID3D12Device* device, 
		DirectX::DescriptorHeap* descHeap, DirectX::ResourceUploadBatch& resUpload) = 0;
	virtual void Render(DirectX::DX12::SpriteBatch* sprite, const DirectX::SimpleMath::Vector2& outputSize) = 0;
};
