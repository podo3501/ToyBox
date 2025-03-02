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

void ScrollBar::SerializeIO(JsonOperation& operation)
{
	UIComponent::SerializeIO(operation);

	if (operation.IsWrite()) return;
	ReloadDatas();
}

unique_ptr<UIComponent> ScrollBar::CreateClone() const
{
	return unique_ptr<ScrollBar>(new ScrollBar(*this));
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

	m_onScrollChangedCB(static_cast<float>(curY) / static_cast<float>(maxRange));
}

void ScrollBar::ChangeSize(const XMUINT2& size) noexcept
{
	m_scrollTrack->ChangeSize(size);
	//크기가 바뀌면 상대적으로 버튼 크기가 정해지기 때문에 조정되어야 한다.
	auto& btnHeight = m_scrollContainer->GetSize().y;
	auto btnSize = XMUINT2{ size.x, min(size.y, btnHeight) };
	m_scrollContainer->ChangeSize(btnSize);
}

void ScrollBar::SetViewContentRatio(float contentRatio) noexcept
{
	//ratio가 1보다 같거나 클 경우에는 스크롤바가 사라지게 해야 할것 같다.
	contentRatio = clamp(contentRatio, 0.2f, 1.f);
	auto& btnSize = m_scrollContainer->GetSize();
	auto sizeY = contentRatio * static_cast<float>(btnSize.y);
	XMUINT2 ratioBtnSize{ btnSize.x, static_cast<uint32_t>(sizeY) };
	m_scrollContainer->ChangeSize(ratioBtnSize);
}

void ScrollBar::SetPositionRatio(float positionRatio) noexcept
{
	positionRatio = clamp(positionRatio, 0.f, 1.f);

	auto relativePosY = static_cast<int32_t>(GetMaxScrollRange<float>() * positionRatio);
	const auto& curPos = m_scrollContainer->GetRelativePosition();
	m_scrollContainer->SetRelativePosition({ curPos.x, relativePosY });
}

unique_ptr<ScrollBar> CreateScrollBar(const UILayout& layout, 
	unique_ptr<UIComponent> scrollTrack,
	unique_ptr<UIComponent> scrollContainer)
{
	unique_ptr<ScrollBar> scrollBar = make_unique<ScrollBar>();
	return scrollBar->Setup(layout, move(scrollTrack), move(scrollContainer)) ? move(scrollBar) : nullptr;
}