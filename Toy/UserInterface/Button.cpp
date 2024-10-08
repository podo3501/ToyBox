#include "pch.h"
#include "Button.h"
#include "../../Include/IRenderer.h"
#include "../Utility.h"
#include "UIType.h"
#include "UILayout.h"
#include "ImagePart.h"

Button::Button(const wstring& resPath) :
	m_resPath{ resPath }
{
	m_layout = make_unique<UILayout>();
}

Button::~Button() = default;

void Button::EachImageParts(function<void(ImagePart*)> action) const
{
	ranges::for_each(m_buttonParts | views::values, [&action](const auto& images) {
		for (const auto& part : images)
			action(part.get());
		});
}

bool Button::EachImageParts(function<bool(ImagePart*)> action)
{
	return ranges::all_of(m_buttonParts | views::values, [&action](const auto& images) {
		for (const auto& part : images)
			ReturnIfFalse(action(part.get()));
		return true;
		});
}

void Button::EachImageParts(Part part, function<void(ImagePart*)> action)
{
	ranges::for_each(m_buttonParts[part], [&action](const auto& part) {
		action(part.get());
		});
}

void Button::EachImageParts(ButtonState btnState, function<void(ImagePart*)> action)
{
	ranges::for_each(m_buttonParts | views::values, [&action, btnState](const auto& images) {
		action(images[btnState].get());
		});
}

bool Button::LoadResources(ILoadData* load)
{
	ReturnIfFalse(EachImageParts([load](ImagePart* part) { return part->Load(load); }));

	SetLocalPosition(); 

	return true;
}

void Button::SetLocalPosition()
{
	const auto& leftSize = m_buttonParts[Part::Left][ButtonState::Normal]->GetSize();
	const auto& middleSize = m_buttonParts[Part::Middle][ButtonState::Normal]->GetSize();
	const auto& rightSize = m_buttonParts[Part::Right][ButtonState::Normal]->GetSize();

	XMUINT2 curMiddleSize{ middleSize };
	curMiddleSize.x = max(0u, static_cast<uint32_t>(m_layout->GetArea().width) - leftSize.x - rightSize.x);
	EachImageParts(Part::Middle, [curMiddleSize](ImagePart* part) { part->SetSize(curMiddleSize); });

	const Vector2& leftLocalPosition{ 0.f, 0.f };
	const Vector2& middleLocalPosition{ static_cast<float>(leftSize.x), 0.f};
	const Vector2& rightLocalPosition{ static_cast<float>(leftSize.x + curMiddleSize.x), 0.f};

	EachImageParts(Part::Left, [&leftLocalPosition](ImagePart* part) { part->SetLocalPosition(leftLocalPosition); });
	EachImageParts(Part::Middle, [&middleLocalPosition](ImagePart* part) { part->SetLocalPosition(middleLocalPosition); });
	EachImageParts(Part::Right, [&rightLocalPosition](ImagePart* part) { part->SetLocalPosition(rightLocalPosition); });
	
	EachImageParts([&origin = m_layout->GetOrigin()](ImagePart* part) { part->MakeLocalDestination(origin); });
}

void Button::Update(const Vector2& resolution, const Mouse::State& mouseState)
{
	//클릭하는 좌표를 LeftTop에서 계산하도록 위치조정한다.
	Vector2 pos{ resolution * m_layout->GetPosition() };
	EachImageParts([&pos](ImagePart* part) { part->SetPosition(pos); });

	bool bOver = false;
	m_state = ButtonState::Normal;
	bOver = ranges::any_of(m_buttonParts | views::values, [btnState = m_state, &mouseState](const auto& images) {
		return images[btnState]->IsOver(mouseState.x, mouseState.y);
		});

	if (!bOver) return;

	if (mouseState.leftButton)
		m_state = ButtonState::Clicked;
	else
		m_state = ButtonState::Over;
}

void Button::Render(IRender* render)
{
	EachImageParts(m_state, [render](ImagePart* part) { part->Render(render); });
}

void Button::SetImage(const vector<wstring>& left, const vector<wstring>& middle, const vector<wstring>& right,
	const Rectangle& area, const Vector2& pos, Origin origin)
{
	m_layout->Set(area, pos, origin);

	SetFilenames(Part::Left, left);
	SetFilenames(Part::Middle, middle);
	SetFilenames(Part::Right, right);
}

void Button::ChangeOrigin(Origin origin)
{
	const Vector2& curOrigin = m_layout->ChangeOrigin(origin);
	EachImageParts([&curOrigin](ImagePart* part) { part->MakeLocalDestination(curOrigin); });
}

void Button::SetFilenames(Part part, const vector<wstring>& filenames)
{
	ranges::transform(filenames, back_inserter(m_buttonParts[part]), [this](const auto& filename) {
		unique_ptr<ImagePart> imagePart = make_unique<ImagePart>(m_resPath + filename);
		return move(imagePart);
		});
}