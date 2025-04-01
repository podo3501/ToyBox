#include "pch.h"
#include "ScrollSlider.h"
#include "ImageGrid1.h"
#include "ImageGrid3.h"
#include "ImageSwitcher.h"
#include "Button.h"
#include "../../../InputManager.h"
#include "../../JsonOperation/JsonOperation.h"
#include "../../../Utility.h"

ScrollSlider::~ScrollSlider() = default;
ScrollSlider::ScrollSlider() :
	m_scrollTrack{ nullptr },
	m_scrollButton{ nullptr }
{}

ScrollSlider::ScrollSlider(const ScrollSlider& other) :
	UIComponent{ other },
	m_scrollTrack{ nullptr },
	m_scrollButton{ nullptr }
{
	ReloadDatas();
}

void ScrollSlider::ReloadDatas() noexcept
{
	vector<UIComponent*> componentList = GetChildComponents();
	m_scrollTrack = ComponentCast<ImageGrid3*>(componentList[0]);
	m_scrollButton = ComponentCast<ImageSwitcher*>(componentList[1]);
	m_scrollButton->AddPressCB([this](KeyState keystate) { OnPressCB(keystate); });
}

bool ScrollSlider::operator==(const UIComponent& rhs) const noexcept
{
	ReturnIfFalse(UIComponent::operator==(rhs));

	const ScrollSlider* o = static_cast<const ScrollSlider*>(&rhs);
	ReturnIfFalse(EqualComponent(m_scrollTrack, o->m_scrollTrack));
	ReturnIfFalse(EqualComponent(m_scrollButton, o->m_scrollButton));

	return true;
}

unique_ptr<UIComponent> ScrollSlider::CreateClone() const
{
	return unique_ptr<ScrollSlider>(new ScrollSlider(*this));
}

bool ScrollSlider::Setup(const UILayout& layout,
	unique_ptr<ImageGrid3> scrollTrack, 
	unique_ptr<ImageSwitcher> scrollButton)
{
	SetLayout(layout);

	m_scrollTrack = scrollTrack.get();
	UIEx(this).AttachComponent(move(scrollTrack), {});
	
	m_scrollButton = scrollButton.get();
	m_scrollButton->AddPressCB([this](KeyState keystate) { OnPressCB(keystate); });
	UIEx(this).AttachComponent(move(scrollButton), {});

	DirectionType dirType = m_scrollTrack->GetDirectionType();
	StateFlag::Type flag = (dirType == DirectionType::Vertical) ? StateFlag::X_SizeLocked : StateFlag::Y_SizeLocked;
	SetStateFlag(flag, true);

	return true;
}

bool ScrollSlider::ImplementBindSourceInfo(TextureSourceBinder*, ITextureController*) noexcept
{
	SetScrollContainerSize(0.5f);	//기본값
	return true;
}

bool ScrollSlider::ImplementUpdate(const DX::StepTimer& timer) noexcept
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

void ScrollSlider::SerializeIO(JsonOperation& operation)
{
	UIComponent::SerializeIO(operation);

	if (operation.IsWrite()) return;
	ReloadDatas();
}

template<typename ReturnType>
ReturnType ScrollSlider::GetMaxScrollRange() const noexcept
{
	const XMUINT2& trackSize = m_scrollTrack->GetSize();
	const XMUINT2& containerSize = m_scrollButton->GetSize();
	return static_cast<ReturnType>(trackSize.y - containerSize.y);
}

void ScrollSlider::OnPressCB(KeyState keyState)
{
	int32_t mPosY = InputManager::GetMouse().GetPosition().y;
	if (keyState == KeyState::Pressed)
	{
		m_pressContainerPos = m_scrollButton->GetRelativePosition();
		m_pressMousePos = mPosY;
		return;
	}
	if (keyState != KeyState::Held) return;

	int32_t moved = mPosY - m_pressMousePos;
	auto maxRange = GetMaxScrollRange<int32_t>();
	auto curY = std::clamp(m_pressContainerPos.y + moved, 0, maxRange);
	m_scrollButton->SetRelativePosition({ m_pressContainerPos.x, curY });

	auto ratio = static_cast<float>(curY) / static_cast<float>(maxRange);
	m_bounded.SetPositionRatio(ratio);
	m_onScrollChangedCB(ratio);
}

bool ScrollSlider::ImplementChangeSize(const XMUINT2& size) noexcept
{
	ReturnIfFalse(m_scrollTrack->ChangeSize(size));
	//크기가 바뀌면 상대적으로 버튼 크기가 정해지기 때문에 조정되어야 한다.
	auto& btnHeight = m_scrollButton->GetSize().y;
	auto btnSize = XMUINT2{ size.x, min(size.y, btnHeight) };
	m_scrollButton->ChangeSize(btnSize);
	return UIComponent::ImplementChangeSize(size);
}

void ScrollSlider::SetScrollContainerSize(float ratio) noexcept
{
	DirectionType dirType = m_scrollTrack->GetDirectionType();
	const auto& area = m_scrollTrack->GetArea();
	XMUINT2 ratioSize = RectangleToXMUINT2(area);

	switch (dirType) {
	case DirectionType::Horizontal: ratioSize.x = static_cast<uint32_t>(area.width * ratio); break;
	case DirectionType::Vertical: ratioSize.y = static_cast<uint32_t>(area.height * ratio); break;
	}

	m_scrollButton->ChangeSize(ratioSize);
}

void ScrollSlider::SetViewContent(uint32_t viewArea, uint32_t contentSize) noexcept
{
	if (int height = viewArea - contentSize; height < 0)
		m_bounded.SetBounds(height, 0, 15);

	SetScrollContainerSize(static_cast<float>(viewArea) / static_cast<float>(contentSize));
}

void ScrollSlider::SetPositionRatio(float positionRatio) noexcept
{
	positionRatio = clamp(positionRatio, 0.f, 1.f);

	auto relativePosY = static_cast<int32_t>(GetMaxScrollRange<float>() * positionRatio);
	const auto& curPos = m_scrollButton->GetRelativePosition();
	m_scrollButton->SetRelativePosition({ curPos.x, relativePosY });
}

void ScrollSlider::SetEnableWheel(bool enable) noexcept
{
	if(!m_isWheelEnabled && enable) ResetMouseWheelValue();
	m_isWheelEnabled = enable;
}

unique_ptr<ScrollSlider> CreateScrollSlider(const UILayout& layout,
	unique_ptr<ImageGrid3> scrollTrack,
	unique_ptr<ImageSwitcher> scrollButton)
{
	unique_ptr<ScrollSlider> scrollSlider = make_unique<ScrollSlider>();
	return CreateIfSetup(move(scrollSlider), layout, move(scrollTrack), move(scrollButton));
}