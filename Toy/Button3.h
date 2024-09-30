#pragma once

#include "../Include/IRenderItem.h"

class Texture;
struct IRenderer;

struct ButtonImage
{
public:
	int heapIndex;
	std::vector<std::wstring> filenames;
};

class Button3 : public IRenderItem
{
	enum class ButtonState
	{
		Normal,
		Over,
		Clicked,
		Count,
	};

public:
	Button3(const std::wstring& resPath);
	virtual ~Button3();

	virtual void OnDeviceLost() override;
	virtual void LoadResources(ID3D12Device* device,
		DirectX::DescriptorHeap* descHeap, DirectX::ResourceUploadBatch& resUpload) override;
	virtual void Render(DirectX::DX12::SpriteBatch* sprite, const DirectX::SimpleMath::Vector2& outputSize) override;

	void LoadResources(IRenderer* renderer);
	void Update(const DirectX::SimpleMath::Vector2& resolution, const DirectX::Mouse::State& state);
	void SetImage(const ButtonImage& normal, const ButtonImage& over, const ButtonImage& clicked,
		const DirectX::SimpleMath::Vector2& pos);
	void Draw(IRenderer* renderer);

private:
	void LoadImage(ID3D12Device* device, DirectX::DescriptorHeap* descHeap, DirectX::ResourceUploadBatch& resUpload,
		ButtonState state);
	void RenderButton(DirectX::DX12::SpriteBatch* sprite, const DirectX::SimpleMath::Vector2& outputSize, 
		const std::vector<std::unique_ptr<Texture>>& textures);

	std::wstring m_resPath{};
	std::map<ButtonState, ButtonImage> m_images;
	std::map<ButtonState, std::vector<std::unique_ptr<Texture>>> m_textures;
	DirectX::SimpleMath::Vector2 m_position{};
	ButtonState m_state{ ButtonState::Normal };
	DirectX::SimpleMath::Vector2 m_origin{};
};
