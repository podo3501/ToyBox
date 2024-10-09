#include "pch.h"
#include "Button.h"
#include "../../Include/IRenderer.h"
#include "../Utility.h"
#include "UIType.h"
#include "UILayout.h"
#include "ImagePart.h"

Button::~Button() = default;
Button::Button() :
	m_layout{ make_unique<UILayout>() }
{}

void Button::EachImageParts(function<void(ImagePart*)> action) const
{
	ranges::for_each(m_NbuttonParts | views::values, [&action](const auto& images) {
		for (const auto& part : images)
			action(part.get());
		});
}

void Button::EachImageParts(ButtonState btnState, function<void(ImagePart*)> action)
{
	ranges::for_each(m_NbuttonParts[btnState], [&action](const auto& part) {
		action(part.get());
		});
}

void Button::EachImageParts(Part part, function<void(ImagePart*)> action)
{
	ranges::for_each(m_NbuttonParts | views::values, [&action, part](const auto& images) {
		action(images[part].get());
		});
}

bool Button::LoadResources(ILoadData* load)
{
	ReturnIfFalse(ranges::all_of(m_NbuttonParts, [this, load](const auto& imageParts) {
		ReturnIfFalse(LoadResources(load, imageParts.second));
		return true;
		}));

	SetLocalPosition();

	return true;
}

bool Button::LoadResources(ILoadData* load, const vector<unique_ptr<ImagePart>>& imageParts )
{
	for (auto& part : imageParts)
		ReturnIfFalse(part->Load(load));
	return true;
}

void Button::SetLocalPosition()
{
	const auto& leftSize = m_NbuttonParts[ButtonState::Normal][Part::Left]->GetSize();
	const auto& centerSize = m_NbuttonParts[ButtonState::Normal][Part::Center]->GetSize();
	const auto& rightSize = m_NbuttonParts[ButtonState::Normal][Part::Right]->GetSize();

	XMUINT2 curCenterSize{ centerSize };
	curCenterSize.x = max(0u, static_cast<uint32_t>(m_layout->GetArea().width) - leftSize.x - rightSize.x);
	EachImageParts(Part::Center, [&curCenterSize](ImagePart* part) { part->SetSize(curCenterSize); });

	const Vector2& leftLocalPosition{ 0.f, 0.f };
	const Vector2& middleLocalPosition{ static_cast<float>(leftSize.x), 0.f };
	const Vector2& rightLocalPosition{ static_cast<float>(leftSize.x + curCenterSize.x), 0.f };

	EachImageParts(Part::Left, [&leftLocalPosition](ImagePart* part) { part->SetLocalPosition(leftLocalPosition); });
	EachImageParts(Part::Center, [&middleLocalPosition](ImagePart* part) { part->SetLocalPosition(middleLocalPosition); });
	EachImageParts(Part::Right, [&rightLocalPosition](ImagePart* part) { part->SetLocalPosition(rightLocalPosition); });

	EachImageParts([&origin = m_layout->GetOrigin()](ImagePart* part) { part->MakeLocalDestination(origin); });
}

void Button::Update(const Vector2& resolution, const Mouse::ButtonStateTracker& tracker)
{
	//클릭하는 좌표를 LeftTop에서 계산하도록 위치조정한다.
	Vector2 pos{ resolution * m_layout->GetPosition() };
	EachImageParts([&pos](ImagePart* part) { part->SetPosition(pos); });

	bool bOver = ranges::any_of(m_NbuttonParts[m_state], [mouseState = tracker.GetLastState()](const auto& part) {
		return part->IsOver(mouseState.x, mouseState.y);
		});

	if (!bOver)
	{
		m_state = ButtonState::Normal;
		return;
	}

	if (tracker.leftButton == Mouse::ButtonStateTracker::PRESSED ||
		m_state == ButtonState::Pressed && tracker.leftButton == Mouse::ButtonStateTracker::HELD)
		m_state = ButtonState::Pressed;
	else
		m_state = ButtonState::Hover;
}

void Button::Render(IRender* render)
{
	EachImageParts(m_state, [render](ImagePart* part) { part->Render(render); });
}

void Button::SetImage(
	const wstring& resPath,
	const vector<ImageSource>& normal,
	const vector<ImageSource>& hover,
	const vector<ImageSource>& pressed,
	const Rectangle& area, const Vector2& pos, Origin origin)
{
	m_layout->Set(area, pos, origin);

	ButtonState btnState = ButtonState::Normal;
	for (const auto& sources : { normal, hover, pressed })
	{
		SetImage(resPath, btnState, sources);
		btnState = static_cast<ButtonState>(static_cast<int>(btnState) + 1);
	}
}

void Button::SetImage(const wstring& resPath, ButtonState btnState, const vector<ImageSource>& sources)
{
	for (const auto& imgSource : sources)
	{
		const wstring& filename = resPath + imgSource.filename;
		ranges::transform(imgSource.list, back_inserter(m_NbuttonParts[btnState]), [&filename](const auto& rect) {
			unique_ptr<ImagePart> imagePart = make_unique<ImagePart>(filename, rect);
			return move(imagePart);
			});
	}
}

void Button::ChangeOrigin(Origin origin)
{
	const Vector2& curOrigin = m_layout->ChangeOrigin(origin);
	EachImageParts([&curOrigin](ImagePart* part) { part->MakeLocalDestination(curOrigin); });
}