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
void Button3::Render(DirectX::DX12::SpriteBatch* sprite)
{
	m_textures[0]->Draw(sprite, DirectX::SimpleMath::Vector2{ (float)m_position.x, (float)m_position.y });
	m_textures[1]->Draw(sprite, DirectX::SimpleMath::Vector2{ (float)m_position.x, (float)m_position.y });
	m_textures[2]->Draw(sprite, DirectX::SimpleMath::Vector2{ (float)m_position.x, (float)m_position.y });
}

void Button3::SetTexture(int index, std::unique_ptr<Texture> tex)
{
	m_textures.insert(std::make_pair(index, std::move(tex)));
}

void Button3::SetImage(const ButtonImage& btnImage, const DirectX::XMUINT2& pos)
{
	m_image = btnImage;
	m_position = pos;
}
