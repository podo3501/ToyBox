#include "pch.h"
#include "Button.h"
#include "../../Utility.h"
#include "../../InputManager.h"
#include "../JsonOperation.h"
#include "ImageGrid1.h"

using enum ButtonState;

Button::~Button() = default;
Button::Button() :
	m_state{ nullopt }
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

bool Button::SetImage(const UILayout& layout,
	unique_ptr<UIComponent>&& normal,
	unique_ptr<UIComponent>&& hover,
	unique_ptr<UIComponent>&& pressed) noexcept
{
	SetLayout(layout);

	AddComponentAndEnable(ButtonState::Normal, move(normal), true);	//Normal이 기본이고 다른 컴포넌트는 비활성시킨다.
	AddComponentAndEnable(ButtonState::Hover, move(hover), false);
	AddComponentAndEnable(ButtonState::Pressed, move(pressed), false);

	SetAttachmentState(AttachmentState::Detach);

	return true;
}

void Button::SetState(ButtonState state) noexcept
{
	if (m_state == state) return;
		
	for (auto& [imgState, image] : m_images)
		image->SetEnable(imgState == state);

	m_state = state;
}

bool Button::ImplementActiveUpdate(const XMINT2& absolutePosition) noexcept
{
	if (!m_state)
	{
		SetState(Normal);
		return true;
	}

	const auto& mouseTracker = InputManager::GetMouse();
	const XMINT2& relativeMousePos = mouseTracker.GetPosition() - absolutePosition;
	if (!IsArea(relativeMousePos))
	{
		SetState(Normal);
		return true;
	}

	bool isPressed = IsInputAction(MouseButton::Left, KeyState::Pressed);
	bool isHeld = IsInputAction(MouseButton::Left, KeyState::Held);

	SetState((isPressed || (*m_state == Pressed && isHeld)) ? Pressed : Hover);
	return true;
}

void Button::ChangeSize(const XMUINT2& size) noexcept
{
	for (const auto& component : GetChildComponents())
		component->ChangeSize(size);
	UIComponent::ChangeSize(size);
}

void Button::AddComponentAndEnable(ButtonState btnState, unique_ptr<UIComponent>&& component, bool enable) noexcept
{
	UIComponent* rawComponent = component.get();
	rawComponent->SetEnable(enable);

	auto grid1 = ComponentCast<ImageGrid1*>(rawComponent);
	rawComponent->SetAttachmentState(grid1 ? AttachmentState::Attach : AttachmentState::Disable);

	m_images.emplace(btnState, rawComponent);
	UIEx(this).AttachComponent(move(component), {});
}

//m_images값은 Button 밑에 달려있는 component인데 읽거나 복사 했을 경우 이 값은 존재하지 않는다. 그럴경우 다시 연결해 준다.
void Button::ReloadDatas() noexcept
{
	vector<UIComponent*> componentList = GetChildComponents();
	m_images.emplace(ButtonState::Normal, componentList[0]);		//여기에 순서가 잘못되면 안된다.
	m_images.emplace(ButtonState::Hover, componentList[1]);
	m_images.emplace(ButtonState::Pressed, componentList[2]);
}

void Button::SerializeIO(JsonOperation& operation)
{
	UIComponent::SerializeIO(operation);

	if (operation.IsWrite()) return;
	ReloadDatas();
}

unique_ptr<Button> CreateButton(const UILayout& layout, vector<unique_ptr<UIComponent>> imgGridList)
{
	if (imgGridList.size() != 3) return nullptr;

	unique_ptr<Button> button = make_unique<Button>();
	auto result = button->SetImage(layout, move(imgGridList[0]), move(imgGridList[1]), move(imgGridList[2]));
	if (!result) return nullptr;

	return button;
}