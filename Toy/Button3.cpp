#include "pch.h"
#include "Button3.h"
#include "Texture.h"

using namespace DirectX;

Button3::Button3(const std::wstring& resPath)
	: m_resPath{ resPath }
{}
Button3::~Button3() = default;

void Button3::OnDeviceLost()
{
	std::ranges::for_each(m_textures | std::views::values, [](auto& tex) {
		tex->Reset();
		});
}

void Button3::LoadResources(ID3D12Device* device,
	DirectX::DescriptorHeap* descHeap, DirectX::ResourceUploadBatch& resUpload)
{
	std::ranges::for_each(m_image.filenames, [&, heaIndex = m_image.heapIndex, texIdx{ 0 }, btn = this]
	(auto& filename) mutable {
			std::unique_ptr<Texture> tex = std::make_unique<Texture>(device, descHeap);
			tex->Upload(resUpload, heaIndex++, btn->m_resPath + filename);
			btn->SetTexture(texIdx++, std::move(tex));
		});
}

void Button3::Render(DX12::SpriteBatch* sprite, const SimpleMath::Vector2& outputSize)
{
	const SimpleMath::Vector2& pos{ outputSize * m_position };

	m_textures[0]->Draw(sprite, pos);
	m_textures[1]->Draw(sprite, pos);
	m_textures[2]->Draw(sprite, pos);
}

void Button3::SetTexture(int index, std::unique_ptr<Texture> tex)
{
	m_textures.insert(std::make_pair(index, std::move(tex)));
}

void Button3::SetImage(const ButtonImage& btnImage, const SimpleMath::Vector2& pos)
{
	m_image = btnImage;
	m_position = pos;
}
