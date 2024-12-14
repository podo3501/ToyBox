#include "pch.h"
#include "OButton.h"
#include "../Utility.h"
#include "../HelperClass.h"
#include "../InputManager.h"
#include "UIType.h"
#include "UILayout.h"
#include "ImagePartSet.h"
#include "JsonOperation.h"

OButton::~OButton() = default;
OButton::OButton() :
	m_state{ ButtonState::Normal }
{}

OButton::OButton(const OButton& other)
	: UIComponent{ other }
{
	ranges::for_each(other.m_image, [this](const auto& pair) {
		ButtonState state = pair.first;
		const auto& imgPartSet = pair.second;
		m_image.insert(make_pair(state, make_unique<ImagePartSet>(*imgPartSet.get())));
		});
}

bool OButton::operator==(const UIComponent& o) const noexcept
{
	ReturnIfFalse(UIComponent::operator==(o));
	const OButton* rhs = static_cast<const OButton*>(&o);

	return CompareAssoc(m_image, rhs->m_image);
}

unique_ptr<UIComponent> OButton::Clone()
{
	return make_unique<OButton>(*this);
}

bool OButton::LoadResources(ILoadData* load)
{
	ReturnIfFalse(ranges::all_of(m_image | views::values, [this, load](const auto& imgPartSet) {
		return imgPartSet->LoadResources(load);
		}));

	//로딩을 다 하고 값을 넣는 이유는 뒤에 로딩하는 것들이 값에 영향을 주기 때문이다.
	auto layout = GetLayout();
	SetDestination(layout->GetArea());

	return true;
}

bool OButton::SetDestination(const Rectangle& area) noexcept
{
	for (const auto& partSet : m_image | views::values)
		ReturnIfFalse(partSet->SetDestination(area));

	return true;
}

bool OButton::ChangeArea(const Rectangle& area) noexcept
{
	ReturnIfFalse(SetDestination(area));

	UIComponent::ChangeArea(area);

	return true;
}

bool OButton::Update(const XMINT2& position, InputManager* inputManager) noexcept
{
	const XMINT2& pos = GetPositionByLayout(position);
	for (const auto& partSet : m_image | views::values)
		ReturnIfFalse(partSet->SetPosition(pos));
	
	if (inputManager == nullptr)
		return true;

	auto tracker = inputManager->GetMouse();
	bool bHover = ranges::any_of(m_image | views::values, [mousePos = tracker->GetOffsetPosition()](const auto& partSet) {
		return partSet->IsHover(mousePos.x, mousePos.y);
		});

	if (!bHover)
	{
		m_state = ButtonState::Normal;
		return true;
	}

	if (tracker->leftButton == Mouse::ButtonStateTracker::PRESSED ||
		m_state == ButtonState::Pressed && tracker->leftButton == Mouse::ButtonStateTracker::HELD)
		m_state = ButtonState::Pressed;
	else
		m_state = ButtonState::Hover;

	return true;
}

void OButton::Render(IRender* render)
{
	m_image[m_state]->Render(render, GetSelected());
}

void OButton::SetImage(const string& name,
	const UILayout& layout,
	const vector<ImageSource>& normal,
	const vector<ImageSource>& hover,
	const vector<ImageSource>& pressed)
{
	SetName(name);
	SetLayout(layout);

	ButtonState btnState = ButtonState::Normal;
	for (const auto& sources : { normal, hover, pressed })
	{
		InsertImage(btnState, make_unique<ImagePartSet>(sources));
		btnState = static_cast<ButtonState>(static_cast<int>(btnState) + 1);
	}
}

void OButton::InsertImage(ButtonState btnState, unique_ptr<ImagePartSet>&& imgPartSet)
{
	m_image.insert(make_pair(btnState, move(imgPartSet)));
}

void OButton::SerializeIO(JsonOperation& operation)
{
	operation.Process("StateImage", m_image);
	UIComponent::SerializeIO(operation);
}