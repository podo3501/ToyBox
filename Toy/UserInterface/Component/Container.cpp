#include "pch.h"
#include "Container.h"
#include "../../InputManager.h"
#include "../JsonOperation.h"
#include "../../Utility.h"

using enum ButtonState;

Container::~Container() = default;
Container::Container() noexcept :
	m_state{ nullopt }
{}

Container::Container(const Container& o) :
	UIComponent{ o },
	m_state{ o.m_state }
{
	ReloadDatas();
}

void Container::ReloadDatas() noexcept
{
	vector<UIComponent*> componentList = GetChildComponents();
	m_images.emplace(Normal, componentList[0]);		//여기에 순서가 잘못되면 안된다.
	m_images.emplace(Hover, componentList[1]);
	m_images.emplace(Pressed, componentList[2]);
}

unique_ptr<UIComponent> Container::CreateClone() const
{
	return unique_ptr<Container>(new Container(*this));
}

bool Container::operator==(const UIComponent& o) const noexcept
{
	ReturnIfFalse(UIComponent::operator==(o));
	const Container* rhs = static_cast<const Container*>(&o);

	return std::all_of(m_images.begin(), m_images.end(), [rhs](const auto& pair) {
		auto state = pair.first;
		return pair.second->GetName() == rhs->m_images.at(state)->GetName();
		});
}

void Container::ClearInteraction() noexcept
{ 
	if (m_state && m_state == ButtonState::Hover)
		SetState(ButtonState::Normal);
}

void Container::ChangeSize(const XMUINT2& size) noexcept
{
	for (const auto& component : GetChildComponents())
		component->ChangeSize(size);
	UIComponent::ChangeSize(size);
}

void Container::AttachComponent(ButtonState btnState, unique_ptr<UIComponent>&& component) noexcept
{
	m_images.emplace(btnState, component.get());
	UIEx(this).AttachComponent(move(component), {});
}

inline static void SetActiveStateFlag(bool condition, UIComponent* component) noexcept
{
	component->SetStateFlag(StateFlag::Active, condition);
}

bool Container::Setup(const UILayout& layout, map<ButtonState, unique_ptr<UIComponent>>&& imgGridList) noexcept
{
	SetLayout(layout);

	for (auto& imgGrid : imgGridList)
	{
		SetActiveStateFlag(imgGrid.first == Normal, imgGrid.second.get());
		AttachComponent(imgGrid.first, move(imgGrid.second));
	}

	return true;
}

void Container::SetState(ButtonState state) noexcept
{
	if (m_state == state) return;

	for (auto& [imgState, image] : m_images)
		SetActiveStateFlag(imgState == state, image);

	m_state = state;
}

bool Container::ImplementUpdatePosition(const DX::StepTimer&, const XMINT2& absolutePos) noexcept
{ 
	if (IsDirty())
		m_position = GetPositionByLayout(absolutePos); //GetPositionByLayout 이걸 안 쓰면 Origin 안될텐데 기존 코드가 이래서... 나중에 확인 ?!?

	return true;
}

bool Container::ImplementActiveUpdate() noexcept
{
	if (!m_state)
	{
		SetState(Normal);
		return true;
	}

	const auto& mouseTracker = InputManager::GetMouse();
	const XMINT2& relativeMousePos = mouseTracker.GetPosition() - m_position;
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

void Container::SerializeIO(JsonOperation& operation)
{
	UIComponent::SerializeIO(operation);

	if (operation.IsWrite()) return;
	ReloadDatas();
}

unique_ptr<Container> CreateContainer(const UILayout& layout, map<ButtonState, unique_ptr<UIComponent>>&& imgGridList)
{
	if (imgGridList.size() != 3) return nullptr;

	unique_ptr<Container> container = make_unique<Container>();
	if(!container->Setup(layout, move(imgGridList))) return nullptr;

	return container;
}