#include "pch.h"
#include "Button.h"
#include "../Include/IRenderer.h"

Button::Button(const wstring& resPath) :
	m_resPath{ resPath }
{}
Button::~Button() = default;

void Button::LoadResources(ILoadData* load)
{
	ranges::for_each(m_images, [this, load](auto& iter) {
		LoadTextures(load, iter.second);
		});
}

void Button::LoadTextures(ILoadData* load, ButtonImage& images)
{
	int idx = images.heapIndex;
	vector<int> xSize;
	ranges::for_each(images.filenames, [this, load, idx, &xSize](const auto& filename) mutable {
		XMUINT2 size{};
		load->LoadTexture(idx++, m_resPath + filename, &size);
		xSize.emplace_back(size.x);
		});

	m_sidePosition = XMFLOAT2{ xSize[0] / 2.0f + xSize[1] / 2.0f, 0.0f };
}

void Button::Update(const Vector2& resolution, const Mouse::State& state)
{
	//클릭하는 좌표를 LeftTop에서 계산하도록 위치조정한다.
	Vector2 pos{ resolution * m_position };
	float areaX = pos.x - m_origin.x;
	float areaY = pos.y - m_origin.y;

	//마우스 좌표를 버튼의 영역으로 바꾼다.
	Vector2 mousePos(static_cast<float>(state.x) - areaX, static_cast<float>(state.y) - areaY);

	bool bOver = m_area.Contains(mousePos);

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
	render->Render(idx+1, XMUINT2(m_area.width, m_area.height), m_images[m_state].position[1], m_origin);
	//render->Render(idx+2, m_images[m_state].position[2]);
}

void Button::SetImage(const ButtonImage& normal, const ButtonImage& over, const ButtonImage& clicked,
	const Rectangle& area, const Vector2& pos, Origin origin)
{
	m_images.insert(make_pair(ButtonState::Normal, normal));
	m_images.insert(make_pair(ButtonState::Over, over));
	m_images.insert(make_pair(ButtonState::Clicked, clicked));

	SetArea(area);
	SetPosition(pos);
	SetOrigin(origin);
}

void Button::SetOrigin(Origin origin)
{ 
	switch (origin)
	{
	case Origin::Center:
		m_origin.x = static_cast<float>(m_area.width) / 2.0f;
		m_origin.y = static_cast<float>(m_area.height) / 2.0f;
		break;
	case Origin::LeftTop:
		m_origin.x = static_cast<float>(m_area.x);
		m_origin.y = static_cast<float>(m_area.y);
		break;
	}
}
