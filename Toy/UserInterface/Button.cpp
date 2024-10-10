#include "pch.h"
#include "Button.h"
#include "../../Include/IRenderer.h"
#include "../Utility.h"
#include "UIType.h"
#include "UILayout.h"
#include "ImagePartSet.h"

Button::~Button() = default;
Button::Button() :
	m_layout{ make_unique<UILayout>() }
{}

bool Button::LoadResources(ILoadData* load)
{
	ReturnIfFalse(ranges::all_of(m_image | views::values, [this, load](const auto& imgPartSet) {
		return imgPartSet->LoadResources(load);
		}));

	for (const auto& partSet : m_image | views::values)
		partSet->SetDestination(m_layout.get());

	return true;
}

void Button::Update(const Vector2& resolution, const Mouse::ButtonStateTracker& tracker)
{
	//클릭하는 좌표를 LeftTop에서 계산하도록 위치조정한다.
	const XMUINT2& origin = m_layout->GetOrigin();
	const Vector2& pos{ resolution * m_layout->GetPosition() };
	XMUINT2 originPos{ static_cast<long>(pos.x) - origin.x, static_cast<long>(pos.y) - origin.y };

	for (const auto& partSet : m_image | views::values)
		partSet->SetPosition(originPos);

	bool bHover = ranges::any_of(m_image | views::values, [mouseState = tracker.GetLastState()](const auto& partSet) {
		return partSet->IsHover(mouseState.x, mouseState.y);
		});

	if (!bHover)
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
	m_image[m_state]->Render(render);
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
		unique_ptr<ImagePartSet> imgPartSet = make_unique<ImagePartSet>(resPath, sources);
		m_image.insert(make_pair(btnState, move(imgPartSet)));
		btnState = static_cast<ButtonState>(static_cast<int>(btnState) + 1);
	}
}

void Button::ChangeOrigin(Origin origin)
{
	m_layout->SetOrigin(origin);
}