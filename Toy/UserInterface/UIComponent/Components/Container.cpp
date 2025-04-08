#include "pch.h"
#include "Container.h"
#include "InputManager.h"
#include "../../JsonOperation/JsonOperation.h"
#include "Utility.h"

using enum InteractState;

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
	m_images.emplace(Hovered, componentList[1]);
	m_images.emplace(Pressed, componentList[2]);
}

unique_ptr<UIComponent> Container::CreateClone() const
{
	return unique_ptr<Container>(new Container(*this));
}

bool Container::operator==(const UIComponent& rhs) const noexcept
{
	ReturnIfFalse(UIComponent::operator==(rhs));
	const Container* o = static_cast<const Container*>(&rhs);

	return std::all_of(m_images.begin(), m_images.end(), [o](const auto& pair) {
		auto state = pair.first;
		return pair.second->GetName() == o->m_images.at(state)->GetName();
		});
}

bool Container::ImplementBindSourceInfo(TextureResourceBinder*, ITextureController*) noexcept
{
	SetState(Normal);
	return true;
}

void Container::ClearInteraction() noexcept
{ 
	if (m_state && m_state == InteractState::Hovered)
		SetState(InteractState::Normal);
}

bool Container::ImplementChangeSize(const XMUINT2& size) noexcept
{
	ReturnIfFalse(ranges::all_of(GetChildComponents(), [&size](const auto& component) {
			return component->ChangeSize(size);
		}));
	return UIComponent::ImplementChangeSize(size);
}

void Container::AttachComponent(InteractState state, unique_ptr<UIComponent>&& component) noexcept
{
	m_images.emplace(state, component.get());
	UIEx(this).AttachComponent(move(component), {});
}

inline static void SetActiveStateFlag(bool condition, UIComponent* component) noexcept
{
	component->SetStateFlag(StateFlag::Active, condition);
}

bool Container::Setup(const UILayout& layout, 
	map<InteractState, unique_ptr<UIComponent>> imgGridList, BehaviorMode behaviorMode) noexcept
{
	SetLayout(layout);
	m_behaviorMode = behaviorMode;

	for (auto& imgGrid : imgGridList)
	{
		SetActiveStateFlag(imgGrid.first == Normal, imgGrid.second.get());
		AttachComponent(imgGrid.first, move(imgGrid.second));
	}

	return true;
}

void Container::SetState(InteractState state) noexcept
{
	if (m_state == state) return;

	for (auto& [imgState, image] : m_images)
		SetActiveStateFlag(imgState == state, image);

	m_state = state;
}

void Container::NormalMode(bool isPressed, bool isHeld) noexcept
{
	if (!Contains(GetArea(), InputManager::GetMouse().GetPosition()))
	{
		SetState(Normal);
		return;
	}

	SetState((isPressed || (m_state == Pressed && isHeld)) ? Pressed : Hovered);
}

//마우스 왼쪽키가 계속 눌러지고 있으면 영역을 벗어나도 눌러지는 state가 유지된다.(scrollbar에서 사용)
void Container::HoldToKeepPressedMode(bool isPressed, bool isHeld) noexcept
{
	if (m_state == Pressed && isHeld) //Pressed가 계속 되고 있다면 리턴한다.
	{
		m_onPressCB(KeyState::Held);
		return; 
	}

	NormalMode(isPressed, isHeld);

	if (m_state == Pressed && isPressed) m_onPressCB(KeyState::Pressed);
}

bool Container::ImplementUpdate(const DX::StepTimer&) noexcept
{
	if (!m_state.has_value()) return true; //로드 하지 않았다면 값이 셋팅되지 않는다.
	//이 두값이 이전프레임과 비교해서 달라졌다면 실행하게 한다면 좀 더 빠르게 된다.
	bool isPressed = IsInputAction(MouseButton::Left, KeyState::Pressed);
	bool isHeld = IsInputAction(MouseButton::Left, KeyState::Held);

	switch (m_behaviorMode) //이 부분은 배열에 함수포인터로 하면 더 빨라지는데 추후 다양한 behavior가 생기면 인자가 달라질수 있기 때문에 일단 보류한다.
	{
	case BehaviorMode::Normal: NormalMode(isPressed, isHeld); break;
	case BehaviorMode::HoldToKeepPressed: HoldToKeepPressedMode(isPressed, isHeld); break;
	}

	return true;
}

void Container::SerializeIO(JsonOperation& operation)
{
	UIComponent::SerializeIO(operation);

	if (operation.IsWrite()) return;
	ReloadDatas();
}

unique_ptr<Container> CreateContainer(const UILayout& layout, map<InteractState, unique_ptr<UIComponent>> imgGridList, BehaviorMode behaviorMode)
{
	if (imgGridList.size() != 3) return nullptr;

	unique_ptr<Container> container = make_unique<Container>();
	return CreateIfSetup(move(container), layout, move(imgGridList), behaviorMode);
}