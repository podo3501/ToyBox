#include "pch.h"
#include "Button3.h"
#include "../Include/IRenderer.h"

using namespace DirectX;

Button3::Button3(const std::wstring& resPath) :
	m_resPath{ resPath }
{}
Button3::~Button3() = default;

void Button3::LoadResources(ILoadData* load)
{
	std::ranges::for_each(m_images, [this, load](auto& iter) {
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

	m_sidePosition = XMFLOAT2{ xSize[0] / 2.0f + xSize[1] / 2.0f, 0.0f };
}

void Button3::Update(const DirectX::SimpleMath::Vector2& resolution, const Mouse::State& state)
{
	SimpleMath::Vector2 pos{ resolution * m_position };

	int x = state.x - static_cast<int>(pos.x);
	int y = state.y - static_cast<int>(pos.y);

	XMINT2 btnSize = m_images[m_state].size;
	bool bOver = (abs(x) < btnSize.x) && (abs(y) < btnSize.y);

	m_state = ButtonState::Normal;

	if (bOver)
		m_state = ButtonState::Over;

	if (m_state == ButtonState::Over && state.leftButton)
		m_state = ButtonState::Clicked;

	m_images[m_state].position.clear();
	m_images[m_state].position.emplace_back(pos - m_sidePosition);
	m_images[m_state].position.emplace_back(pos);
	m_images[m_state].position.emplace_back(pos + m_sidePosition);
}

void Button3::Render(IRender* render)
{
	int idx = m_images[m_state].heapIndex;
	render->Render(idx+0, m_images[m_state].position[0]);
	render->Render(idx+1, m_images[m_state].position[1]);
	render->Render(idx+2, m_images[m_state].position[2]);
}

void Button3::SetImage(const ButtonImage& normal, const ButtonImage& over, const ButtonImage& clicked,
	const XMFLOAT2& pos)
{
	m_images.insert(std::make_pair(ButtonState::Normal, normal));
	m_images.insert(std::make_pair(ButtonState::Over, over));
	m_images.insert(std::make_pair(ButtonState::Clicked, clicked));

	m_position = pos;
}
