#include "pch.h"
#include "ScrollBar.h"
#include "ImageGrid3.h"
#include "Button.h"
#include "Container.h"
#include "../../InputManager.h"
#include "../JsonOperation.h"
#include "../../Utility.h"

ScrollBar::~ScrollBar() = default;
ScrollBar::ScrollBar() :
	m_scrollTrack{ nullptr },
	m_scrollContainer{ nullptr }
{}

ScrollBar::ScrollBar(const ScrollBar& other) :
	UIComponent{ other },
	m_scrollTrack{ nullptr },
	m_scrollContainer{ nullptr }
{
	ReloadDatas();
}

void ScrollBar::ReloadDatas() noexcept
{
	vector<UIComponent*> componentList = GetChildComponents();
	m_scrollTrack = ComponentCast<ImageGrid3*>(componentList[0]);
	m_scrollContainer = ComponentCast<Container*>(componentList[1]);
}

bool ScrollBar::operator==(const UIComponent& rhs) const noexcept
{
	ReturnIfFalse(UIComponent::operator==(rhs));

	const ScrollBar* o = static_cast<const ScrollBar*>(&rhs);
	ReturnIfFalse(EqualComponent(m_scrollTrack, o->m_scrollTrack));
	ReturnIfFalse(EqualComponent(m_scrollContainer, o->m_scrollContainer));

	return true;
}

unique_ptr<UIComponent> ScrollBar::CreateClone() const
{
	return unique_ptr<ScrollBar>(new ScrollBar(*this));
}

bool ScrollBar::Setup(const UILayout& layout, unique_ptr<UIComponent> scrollTrack, unique_ptr<UIComponent> scrollContainer)
{
	SetLayout(layout);

	m_scrollTrack = ComponentCast<ImageGrid3*>(scrollTrack.get());
	UIEx(this).AttachComponent(move(scrollTrack), {});
	
	m_scrollContainer = ComponentCast<Container*>(scrollContainer.get());
	m_scrollContainer->AddPressCB([this](KeyState keystate) { OnPressCB(keystate); });
	UIEx(this).AttachComponent(move(scrollContainer), {});

	return true;
}

bool ScrollBar::ImplementUpdate(const DX::StepTimer& timer) noexcept
{
	int wheelValue{ 0 };
	if (m_isWheelEnabled)
		wheelValue = GetMouseWheelValue();

	if (!m_bounded.ValidateRange(wheelValue, timer)) return true;

	auto posRatio = m_bounded.GetPositionRatio();
	SetPositionRatio(posRatio);
	m_onScrollChangedCB(posRatio);

	return true;
}

void ScrollBar::SerializeIO(JsonOperation& operation)
{
	UIComponent::SerializeIO(operation);

	if (operation.IsWrite()) return;
	ReloadDatas();
}

template<typename ReturnType>
ReturnType ScrollBar::GetMaxScrollRange() const noexcept
{
	const XMUINT2& trackSize = m_scrollTrack->GetSize();
	const XMUINT2& containerSize = m_scrollContainer->GetSize();
	return static_cast<ReturnType>(trackSize.y - containerSize.y);
}

void ScrollBar::OnPressCB(KeyState keyState)
{
	static int32_t startPosY{ 0 };
	const auto& mPos = InputManager::GetMouse().GetPosition();
	if (keyState == KeyState::Pressed)
	{
		startPosY = mPos.y;
		return;
	}

	int32_t moved = mPos.y - startPosY;
	if (keyState != KeyState::Held || !moved) return;
	
	const auto& containerPos = m_scrollContainer->GetRelativePosition();
	auto maxRange = GetMaxScrollRange<int32_t>();
	auto curY = clamp(containerPos.y + moved, 0, maxRange);
	m_scrollContainer->SetRelativePosition({ containerPos.x, curY });
	startPosY = mPos.y;

	auto ratio = static_cast<float>(curY) / static_cast<float>(maxRange);
	m_bounded.SetPositionRatio(ratio);
	m_onScrollChangedCB(ratio);
}

bool ScrollBar::ChangeSize(const XMUINT2& size) noexcept
{
	ReturnIfFalse(m_scrollTrack->ChangeSize(size));
	//크기가 바뀌면 상대적으로 버튼 크기가 정해지기 때문에 조정되어야 한다.
	auto& btnHeight = m_scrollContainer->GetSize().y;
	auto btnSize = XMUINT2{ size.x, min(size.y, btnHeight) };
	return m_scrollContainer->ChangeSize(btnSize);
}

void ScrollBar::SetScrollContainerSize(float ratio) noexcept
{
	DirectionType dirType = m_scrollTrack->GetDirectionType();
	const auto& area = m_scrollTrack->GetArea();
	XMUINT2 ratioSize = RectangleToXMUINT2(area);

	switch (dirType) {
	case DirectionType::Horizontal: ratioSize.x = static_cast<uint32_t>(area.width * ratio); break;
	case DirectionType::Vertical: ratioSize.y = static_cast<uint32_t>(area.height * ratio); break;
	}

	m_scrollContainer->ChangeSize(ratioSize);
}

void ScrollBar::SetViewContent(uint32_t viewArea, uint32_t contentSize) noexcept
{
	//ratio가 1보다 같거나 클 경우에는 스크롤바가 사라지게 해야 할것 같다.
	if (int height = viewArea - contentSize; height < 0)
		m_bounded.SetBounds(height, 0, 15);

	SetScrollContainerSize(static_cast<float>(viewArea) / static_cast<float>(contentSize));
}

void ScrollBar::SetPositionRatio(float positionRatio) noexcept
{
	positionRatio = clamp(positionRatio, 0.f, 1.f);

	auto relativePosY = static_cast<int32_t>(GetMaxScrollRange<float>() * positionRatio);
	const auto& curPos = m_scrollContainer->GetRelativePosition();
	m_scrollContainer->SetRelativePosition({ curPos.x, relativePosY });
}

void ScrollBar::SetEnableWheel(bool enable) noexcept
{
	if(!m_isWheelEnabled && enable) ResetMouseWheelValue();
	m_isWheelEnabled = enable;
}

unique_ptr<ScrollBar> CreateScrollBar(const UILayout& layout, 
	unique_ptr<UIComponent> scrollTrack,
	unique_ptr<UIComponent> scrollContainer)
{
	unique_ptr<ScrollBar> scrollBar = make_unique<ScrollBar>();
	return scrollBar->Setup(layout, move(scrollTrack), move(scrollContainer)) ? move(scrollBar) : nullptr;
}