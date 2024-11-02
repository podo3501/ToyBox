#include "pch.h"
#include "Button.h"
#include "../../Include/IRenderer.h"
#include "../Utility.h"
#include "UIType.h"
#include "UILayout.h"
#include "ImagePartSet.h"

const Rectangle& Button::GetArea() const noexcept { return m_layout->GetArea(); }

Button::~Button() = default;
Button::Button() :
	m_layout{ nullptr }
{}

bool Button::LoadResources(ILoadData* load)
{
	ReturnIfFalse(ranges::all_of(m_image | views::values, [this, load](const auto& imgPartSet) {
		return imgPartSet->LoadResources(load);
		}));

	//�ε��� �� �ϰ� ���� �ִ� ������ �ڿ� �ε��ϴ� �͵��� ���� ������ �ֱ� �����̴�.
	SetDestination(m_layout->GetArea());

	return true;
}

bool Button::SetDestination(const Rectangle& area) noexcept
{
	for (const auto& partSet : m_image | views::values)
		ReturnIfFalse(partSet->SetDestination(area));

	return true;
}

bool Button::ChangeArea(Rectangle&& area) noexcept
{
	ReturnIfFalse(SetDestination(area));

	m_layout->Set(move(area));

	return true;
}

void Button::Update(const Vector2& resolution, const Mouse::ButtonStateTracker* tracker) noexcept
{
	const Vector2& originPos = m_layout->GetPosition(resolution);

	for (const auto& partSet : m_image | views::values)
		partSet->SetPosition(originPos);

	bool bHover = ranges::any_of(m_image | views::values, [mouseState = tracker->GetLastState()](const auto& partSet) {
		return partSet->IsHover(mouseState.x, mouseState.y);
		});

	if (!bHover)
	{
		m_state = ButtonState::Normal;
		return;
	}

	if (tracker->leftButton == Mouse::ButtonStateTracker::PRESSED ||
		m_state == ButtonState::Pressed && tracker->leftButton == Mouse::ButtonStateTracker::HELD)
		m_state = ButtonState::Pressed;
	else
		m_state = ButtonState::Hover;
}

void Button::Render(IRender* render)
{
	m_image[m_state]->Render(render);
}

void Button::SetImage(
	IRenderer* renderer,
	const vector<ImageSource>& normal,
	const vector<ImageSource>& hover,
	const vector<ImageSource>& pressed,
	const UILayout& layout)
{
	m_layout = make_unique<UILayout>(layout);

	ButtonState btnState = ButtonState::Normal;
	for (const auto& sources : { normal, hover, pressed })
	{
		unique_ptr<ImagePartSet> imgPartSet = make_unique<ImagePartSet>(sources);
		m_image.insert(make_pair(btnState, move(imgPartSet)));
		btnState = static_cast<ButtonState>(static_cast<int>(btnState) + 1);
	}

	renderer->AddLoadResource(this);
}

void Button::ChangeOrigin(Origin&& origin) noexcept
{
	m_layout->Set(move(origin));
}