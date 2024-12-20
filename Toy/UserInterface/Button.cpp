#include "pch.h"
#include "Button.h"
#include "UIType.h"
#include "../Utility.h"
#include "../InputManager.h"
#include "../HelperClass.h"
#include "JsonOperation.h"

Button::~Button() = default;
Button::Button() :
	m_state{ ButtonState::Normal }
{}

Button::Button(const Button& o) :
	UIComponent{ o },
	m_state{ o.m_state }
{
	ReloadDatas();
}

unique_ptr<UIComponent> Button::CreateClone() const
{
	return unique_ptr<Button>(new Button(*this));
}

bool Button::operator==(const UIComponent& o) const noexcept
{
	ReturnIfFalse(UIComponent::operator==(o));
	const Button* rhs = static_cast<const Button*>(&o);

	return std::all_of(m_images.begin(), m_images.end(), [rhs](const auto& pair) {
		auto state = pair.first;
		return pair.second->GetName() == rhs->m_images.at(state)->GetName();
		});
}

bool Button::LoadResources(ILoadData* load)
{
	ReturnIfFalse(ranges::all_of(m_images | views::values, [this, load](const auto& image) {
		return image->LoadResources(load);
		}));

	return true;
}

bool Button::SetImage(const string& name, const UILayout& layout,
	unique_ptr<UIComponent>&& normal,
	unique_ptr<UIComponent>&& hover,
	unique_ptr<UIComponent>&& pressed)
{
	SetName(name);
	SetLayout(layout);

	AddComponentAndEnable(ButtonState::Normal, move(normal), true);
	AddComponentAndEnable(ButtonState::Hover, move(hover), false);
	AddComponentAndEnable(ButtonState::Pressed, move(pressed), false);

	return true;
}

void Button::EnableButtonImage(ButtonState btnState)
{
	for (auto& [state, image] : m_images)
		image->SetEnable(state == btnState);
}

bool Button::UpdateButton(const XMINT2&, InputManager* inputManager) noexcept
{
	if (inputManager == nullptr)
		return true;

	auto tracker = inputManager->GetMouse();
	bool isArea = IsArea(tracker->GetOffsetPosition());

	if (!isArea)
		m_state = ButtonState::Normal;
	else
	{
		if (tracker->leftButton == Mouse::ButtonStateTracker::PRESSED ||
			m_state == ButtonState::Pressed && tracker->leftButton == Mouse::ButtonStateTracker::HELD)
			m_state = ButtonState::Pressed;
		else
			m_state = ButtonState::Hover;
	}

	EnableButtonImage(m_state);

	return true;
}

bool Button::Update(const XMINT2& position, InputManager* inputManager) noexcept
{
	UpdateButton(position, inputManager);

	return true;
}

bool Button::ChangeArea(const Rectangle& area) noexcept
{
	const vector<UIComponent*> components = GetComponents();
	for (const auto& component : components)
		component->ChangeArea(area);
	UIComponent::ChangeArea(area);

	return true;
}

void Button::AddComponentAndEnable(ButtonState btnState, unique_ptr<UIComponent>&& component, bool enable)
{
	component->SetEnable(enable);
	m_images.emplace(btnState, component.get());
	AddComponent(move(component), {});
}

//m_images값은 Button 밑에 달려있는 component인데 읽거나 복사 했을 경우 이 값은 존재하지 않는다. 그럴경우 다시 연결해 준다.
void Button::ReloadDatas() noexcept
{
	vector<UIComponent*> componentList = GetComponents();
	m_images.emplace(ButtonState::Normal, componentList[0]);		//여기에 순서가 잘못되면 안된다.
	m_images.emplace(ButtonState::Hover, componentList[1]);
	m_images.emplace(ButtonState::Pressed, componentList[2]);

	EnableButtonImage(m_state);
}

void Button::SerializeIO(JsonOperation& operation)
{
	UIComponent::SerializeIO(operation);

	if (operation.IsWrite()) return;
	ReloadDatas();
}