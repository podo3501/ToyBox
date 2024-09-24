#pragma once

struct ID3D12Device;
namespace DirectX
{
	class DescriptorHeap;
	class ResourceUploadBatch;
	namespace DX12
	{
		class SpriteBatch;
	}
}

struct IRenderItem
{
public:
	virtual ~IRenderItem() {};
	virtual void OnDeviceLost() = 0;
	virtual void LoadResources(ID3D12Device* device, 
		DirectX::DescriptorHeap* descHeap, DirectX::ResourceUploadBatch& resUpload) = 0;
	virtual void Render(DirectX::DX12::SpriteBatch* sprite) = 0;
};
