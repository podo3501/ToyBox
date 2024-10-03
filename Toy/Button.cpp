#include "pch.h"
#include "Button.h"
#include "../Include/IRenderer.h"

using namespace DirectX;

Button::Button(const std::wstring& resPath) :
	m_resPath{ resPath }
{}
Button::~Button() = default;

void Button::LoadResources(ILoadData* load)
{
	std::ranges::for_each(m_images, [this, load](auto& iter) {
		LoadTextures(load, iter.second);
		});
}

void Button::LoadTextures(ILoadData* load, ButtonImage& images)
{
	//XMINT2 buttonSize{};
	int idx = images.heapIndex;
	std::vector<int> xSize;
	std::ranges::for_each(images.filenames, [this, load, idx, &xSize](const auto& filename) mutable {
		XMUINT2 size{};
		load->LoadTexture(idx++, m_resPath + filename, &size);
		//buttonSize.x += size.x;
		//buttonSize.y = size.y;
		xSize.emplace_back(size.x);
		});
	//buttonSize.x = buttonSize.x / 2;
	//buttonSize.y = buttonSize.y / 2;
	//images.size = buttonSize;

	m_sidePosition = XMFLOAT2{ xSize[0] / 2.0f + xSize[1] / 2.0f, 0.0f };
}

void Button::Update(const DirectX::SimpleMath::Vector2& resolution, const Mouse::State& state)
{
	//클릭하는 좌표영역의 시작점은 LeftTop에서 RightBottom으로 맞춘다.
	//나중에 RECT로 바꾸자
	Vector2 pos{ resolution * m_position };
	int areaX = static_cast<int>(pos.x - m_origin.x);
	int areaY = static_cast<int>(pos.y - m_origin.y);

	//마우스 좌표를 버튼의 영역으로 바꾼다.
	XMINT2 mousePos{ state.x - areaX, state.y - areaY };

	bool bOver = (0 <= mousePos.x && mousePos.x < static_cast<int>(m_size.x)) &&
		(0 <= mousePos.y && mousePos.y < static_cast<int>(m_size.y));

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

void Button::Render(IRender* render)
{
	int idx = m_images[m_state].heapIndex;
	//render->Render(idx+0, m_images[m_state].position[0]);
	render->Render(idx+1, m_images[m_state].position[1], m_origin);
	//render->Render(idx+2, m_images[m_state].position[2]);
}

void Button::SetImage(const ButtonImage& normal, const ButtonImage& over, const ButtonImage& clicked,
	const XMUINT2& size, const XMFLOAT2& pos, Origin origin)
{
	m_images.insert(std::make_pair(ButtonState::Normal, normal));
	m_images.insert(std::make_pair(ButtonState::Over, over));
	m_images.insert(std::make_pair(ButtonState::Clicked, clicked));

	SetSize(size);
	SetPosition(pos);
	SetOrigin(origin);
}

void Button::SetOrigin(Origin origin)
{ 
	switch (origin)
	{
	case Origin::Center:
		m_origin.x = static_cast<float>(m_size.x) / 2.0f;
		m_origin.y = static_cast<float>(m_size.y) / 2.0f;
		break;
	case Origin::LeftTop:
		m_origin.x = static_cast<float>(m_size.x);
		m_origin.y = static_cast<float>(m_size.y);
		break;
	}
}
