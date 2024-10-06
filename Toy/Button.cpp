#include "pch.h"
#include "Button.h"
#include "../Include/IRenderer.h"
#include "ImagePart.h"
#include "Utility.h"

Button::Button(const wstring& resPath) :
	m_resPath{ resPath }
{}
Button::~Button() = default;

bool Button::LoadResources(ILoadData* load)
{
	return ranges::all_of(m_buttonParts, [this, load](auto& iter) {
		return LoadTextures(load, iter.second);
		});
}

bool Button::LoadTextures(ILoadData* load, const vector<unique_ptr<ImagePart>>& imageParts)
{
	for (const auto& part : imageParts)
		ReturnIfFalse(part->Load(load));

	const auto& leftSize = imageParts[Part::Left]->GetSize();
	const auto& rightSize = imageParts[Part::Right]->GetSize();
	
	XMUINT2 middleSize{};
	middleSize.x = max(0u, static_cast<uint32_t>(m_area.width) - leftSize.x - rightSize.x);
	middleSize.y = imageParts[Part::Middle]->GetSize().y;
	imageParts[Part::Middle]->SetSize(middleSize);
	
	imageParts[Part::Left]->SetLocalPosition({ 0.f, 0.f });
	imageParts[Part::Middle]->SetLocalPosition({ static_cast<float>(leftSize.x), 0.f });
	imageParts[Part::Right]->SetLocalPosition({ static_cast<float>(leftSize.x + middleSize.x), 0.f });

	return true;
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

	for (const auto& part : m_buttonParts[m_state])
		part->SetPosition(pos);
}

void Button::Render(IRender* render)
{
	for (const auto& part : m_buttonParts[m_state])
		part->Render(render, m_origin);
}

void Button::SetImage(const ButtonImage& normal, const ButtonImage& over, const ButtonImage& clicked,
	const Rectangle& area, const Vector2& pos, Origin origin)
{
	SetArea(area);
	SetPosition(pos);
	SetOrigin(origin);

	//잘라진 이미지들
	SetFilenames(ButtonState::Normal, normal);
	SetFilenames(ButtonState::Over, over);
	SetFilenames(ButtonState::Clicked, clicked);
}

void Button::SetFilenames(ButtonState state, const ButtonImage& images)
{
	ranges::transform(images.filenames, back_inserter(m_buttonParts[state]), [this](const auto& filename) {
		unique_ptr<ImagePart> imagePart = make_unique<ImagePart>(m_resPath + filename);
		return move(imagePart);
		});
}

Vector2 Button::GetOrigin(Origin origin) const noexcept
{
	switch (origin)
	{
	case Origin::Center: return Vector2(static_cast<float>(m_area.width), static_cast<float>(m_area.height)) / 2.0f;
	case Origin::LeftTop: return  Vector2(0.f, 0.f);
	}
	return Vector2(0.f, 0.f);
}

void Button::SetOrigin(Origin origin)
{ 
	m_origin = GetOrigin(origin);
}
