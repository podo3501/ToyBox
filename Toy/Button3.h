#pragma once

#include "../Include/IRenderItem.h"

class Texture;

struct ButtonImage
{
public:
	int heapIndex;
	std::vector<std::wstring> filenames;
};

class Button3 : public IRenderItem
{
public:
	Button3(const std::wstring& resPath);
	virtual ~Button3();

	virtual void OnDeviceLost() override;
	virtual void LoadResources(ID3D12Device* device,
		DirectX::DescriptorHeap* descHeap, DirectX::ResourceUploadBatch& resUpload) override;
	virtual void Render(DirectX::DX12::SpriteBatch* sprite, const DirectX::SimpleMath::Vector2& outputSize) override;

	void SetTexture(int index, std::unique_ptr<Texture> tex);
	void SetImage(const ButtonImage& btnImage, const DirectX::SimpleMath::Vector2& pos);

private:
	ButtonImage m_image{};
	std::wstring m_resPath{};
	std::map<int, std::unique_ptr<Texture>> m_textures;
	DirectX::SimpleMath::Vector2 m_position{};
};
