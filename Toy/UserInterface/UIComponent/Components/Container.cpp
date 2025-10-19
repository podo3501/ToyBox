#include "pch.h"
#include "Container.h"
#include "Locator/InputLocator.h"
#include "Shared/SerializerIO/SerializerIO.h"
#include "Shared/Utils/GeometryExt.h"
#include "Shared/Utils/StlExt.h"

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
	m_textures.emplace(Normal, componentList[0]);		//여기에 순서가 잘못되면 안된다.
	m_textures.emplace(Hovered, componentList[1]);
	m_textures.emplace(Pressed, componentList[2]);
}

unique_ptr<UIComponent> Container::CreateClone() const
{
	return unique_ptr<Container>(new Container(*this));
}

bool Container::operator==(const UIComponent& rhs) const noexcept
{
	ReturnIfFalse(UIComponent::operator==(rhs));
	const Container* o = static_cast<const Container*>(&rhs);

	//return ranges::all_of(m_textures, [o](const auto& pair) {
	//	auto state = pair.first;
	//	return pair.second->GetName() == o->m_textures.at(state)->GetName();
	//	});
	return CompareAssoc(m_textures, o->m_textures);
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

bool Container::ImplementChangeSize(const XMUINT2& size, bool isForce) noexcept
{
	return ranges::all_of(GetChildComponents(), [&size, isForce](const auto& component) {
			return component->ChangeSize(size, isForce); });
}

void Container::AttachComponent(InteractState state, unique_ptr<UIComponent>&& component) noexcept
{
	m_textures.emplace(state, component.get());
	UIEx(this).AttachComponent(move(component), {});
}

inline static void SetActiveStateFlag(bool condition, UIComponent* component) noexcept
{
	component->SetStateFlag(StateFlag::Active, condition);
}

bool Container::Setup(const UILayout& layout, 
	map<InteractState, unique_ptr<UIComponent>> patchTexList, BehaviorMode behaviorMode) noexcept
{
	SetLayout(layout);
	m_behaviorMode = behaviorMode;

	for (auto& pTex : patchTexList)
	{
		SetActiveStateFlag(pTex.first == Normal, pTex.second.get());
		AttachComponent(pTex.first, move(pTex.second));
	}

	return true;
}

void Container::SetState(InteractState curState) noexcept
{
	if (m_state == curState) return;

	for (auto& [state, tex] : m_textures)
		SetActiveStateFlag(state == curState, tex);

	m_state = curState;
}

void Container::NormalMode(bool isPressed, bool isHeld) noexcept
{
	auto input = InputLocator::GetService();
	if (!Contains(GetArea(), input->GetPosition()))
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
		m_onPressCB(InputState::Held);
		return; 
	}

	NormalMode(isPressed, isHeld);

	if (m_state == Pressed && isPressed) m_onPressCB(InputState::Pressed);
}

bool Container::ImplementUpdate(const DX::StepTimer&) noexcept
{
	if (!m_state.has_value()) return true; //로드 하지 않았다면 값이 셋팅되지 않는다.
	//이 두값이 이전프레임과 비교해서 달라졌다면 실행하게 한다면 좀 더 빠르게 된다.
	auto input = InputLocator::GetService();
	bool isPressed = input->IsInputAction(MouseButton::Left, InputState::Pressed);
	bool isHeld = input->IsInputAction(MouseButton::Left, InputState::Held);

	switch (m_behaviorMode) //이 부분은 배열에 함수포인터로 하면 더 빨라지는데 추후 다양한 behavior가 생기면 인자가 달라질수 있기 때문에 일단 보류한다.
	{
	case BehaviorMode::Normal: NormalMode(isPressed, isHeld); break;
	case BehaviorMode::HoldToKeepPressed: HoldToKeepPressedMode(isPressed, isHeld); break;
	}

	return true;
}

void Container::ProcessIO(SerializerIO& serializer)
{
	UIComponent::ProcessIO(serializer);

	if (serializer.IsWrite()) return;
	ReloadDatas();
}