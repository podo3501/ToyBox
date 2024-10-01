#include "pch.h"
#include "Button3.h"
#include "../Include/IRenderer.h"

using namespace DirectX;

Button3::Button3(const std::wstring& resPath, const SimpleMath::Vector2& resolution) :
	m_resPath{ resPath },
	m_resolution{ resolution }
{}
Button3::~Button3() = default;

void Button3::LoadResources(ILoadData* load)
{
	std::ranges::for_each(m_images, [this, load](auto& iter) {
		//ButtonState btnState = iter.first;
		LoadTextures(load, iter.second);
		});
}

void Button3::LoadTextures(ILoadData* load, ButtonImage& images)
{
	XMINT2 buttonSize{};
	int idx = images.heapIndex;
	std::vector<int> xSize;
	std::ranges::for_each(images.filenames, [this, load, idx, &buttonSize, &xSize](const auto& filename) mutable {
		XMUINT2 size{};
		load->LoadTexture(idx++, m_resPath + filename, &size);
		buttonSize.x += size.x;
		buttonSize.y = size.y;
		xSize.emplace_back(size.x);
		});
	buttonSize.x = buttonSize.x / 2;
	buttonSize.y = buttonSize.y / 2;
	images.size = buttonSize;

	SimpleMath::Vector2 pos{ m_resolution * m_position };
	float leftX = pos.x - (xSize[0] / 2.0f + xSize[1] / 2.0f);
	float rightX = pos.x + xSize[1] / 2.0f + xSize[2] / 2.0f;

	images.position.emplace_back(leftX, pos.y);
	images.position.emplace_back(pos);
	images.position.emplace_back(rightX, pos.y);
}
//
//void Button3::LoadResources(ID3D12Device* device,
//	DirectX::DescriptorHeap* descHeap, DirectX::ResourceUploadBatch& resUpload)
//{
//	LoadImage(device, descHeap, resUpload, ButtonState::Normal);
//	LoadImage(device, descHeap, resUpload, ButtonState::Over);
//	LoadImage(device, descHeap, resUpload, ButtonState::Clicked);
//
//	m_origin.x = m_origin.x / 6.0f;
//	m_origin.y = m_origin.y / 2.0f;
//}
//
//void Button3::LoadImage(ID3D12Device* device, DirectX::DescriptorHeap* descHeap, 
//	DirectX::ResourceUploadBatch& resUpload, ButtonState state)
//{
//	const ButtonImage& curImg = m_images[state];
//	std::ranges::for_each(curImg.filenames, [&, heaIndex = curImg.heapIndex, btn = this]
//	(auto& filename) mutable {
//			std::unique_ptr<Texture> tex = std::make_unique<Texture>(device, descHeap);
//			tex->Upload(resUpload, heaIndex++, btn->m_resPath + filename);
//			btn->m_origin.x += static_cast<float>(tex->GetSize().x);
//			btn->m_origin.y = static_cast<float>(tex->GetSize().y);
//			btn->m_textures[state].emplace_back(std::move(tex));
//		});
//}

void Button3::Update(const Mouse::State& state)
{
	SimpleMath::Vector2 pos{ m_resolution * m_position };

	int x = state.x - static_cast<int>(pos.x);
	int y = state.y - static_cast<int>(pos.y);

	XMINT2 btnSize = m_images[m_state].size;
	bool bOver = (abs(x) < btnSize.x) && (abs(y) < btnSize.y);

	m_state = ButtonState::Normal;

	if (bOver)
		m_state = ButtonState::Over;

	if (m_state == ButtonState::Over && state.leftButton)
		m_state = ButtonState::Clicked;
}

void Button3::Render(IRenderer* renderer)
{
	int idx = m_images[m_state].heapIndex;
	renderer->Render(idx+0, m_images[m_state].position[0]);
	renderer->Render(idx+1, m_images[m_state].position[1]);
	renderer->Render(idx+2, m_images[m_state].position[2]);
}

//void Button3::Render(DX12::SpriteBatch* sprite, const SimpleMath::Vector2& outputSize)
//{
//	RenderButton(sprite, outputSize, m_textures[m_state]);
//}
//
//void Button3::RenderButton(DX12::SpriteBatch* sprite, const SimpleMath::Vector2& outputSize,
//	const std::vector<std::unique_ptr<Texture>>& textures)
//{
//	const SimpleMath::Vector2& pos{ outputSize * m_position };
//	const auto& middleSize = textures[1]->GetSize();
//	SimpleMath::Vector2 leftPos = { pos.x - static_cast<float>(middleSize.x) / 2.0f - 12, pos.y };
//	SimpleMath::Vector2 rightPos = { pos.x + static_cast<float>(middleSize.x) / 2.0f + 12, pos.y };
//
//	textures[0]->Draw(sprite, leftPos);
//	textures[1]->Draw(sprite, pos);
//	textures[2]->Draw(sprite, rightPos);
//}

void Button3::SetImage(const ButtonImage& normal, const ButtonImage& over, const ButtonImage& clicked,
	const SimpleMath::Vector2& pos)
{
	m_images.insert(std::make_pair(ButtonState::Normal, normal));
	m_images.insert(std::make_pair(ButtonState::Over, over));
	m_images.insert(std::make_pair(ButtonState::Clicked, clicked));

	m_position = pos;
}
