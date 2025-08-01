#include "pch.h"
#include "ScrollBar.h"
#include "PatchTexture/PatchTextureStd/PatchTextureStd1.h"
#include "PatchTexture/PatchTextureStd/PatchTextureStd3.h"
#include "TextureSwitcher.h"
#include "Button.h"
#include "InputManager.h"
#include "../../JsonOperation/JsonOperation.h"
#include "Utils/GeometryUtil.h"

ScrollBar::~ScrollBar() = default;
ScrollBar::ScrollBar() :
	m_scrollTrack{ nullptr },
	m_scrollButton{ nullptr }
{
	SetRenderTraversal(RenderTraversal::DFS);
}

ScrollBar::ScrollBar(const ScrollBar& other) :
	UIComponent{ other },
	m_scrollTrack{ nullptr },
	m_scrollButton{ nullptr }
{
	ReloadDatas();
}

void ScrollBar::ReloadDatas() noexcept
{
	vector<UIComponent*> componentList = GetChildComponents();
	m_scrollTrack = ComponentCast<PatchTextureStd3*>(componentList[0]);
	m_scrollButton = ComponentCast<TextureSwitcher*>(componentList[1]);
	m_scrollButton->AddPressCB([this](KeyState keystate) { OnPressCB(keystate); });
}

bool ScrollBar::operator==(const UIComponent& rhs) const noexcept
{
	ReturnIfFalse(UIComponent::operator==(rhs));

	const ScrollBar* o = static_cast<const ScrollBar*>(&rhs);
	ReturnIfFalse(EqualComponent(m_scrollTrack, o->m_scrollTrack));
	ReturnIfFalse(EqualComponent(m_scrollButton, o->m_scrollButton));

	return true;
}

unique_ptr<UIComponent> ScrollBar::CreateClone() const
{
	return unique_ptr<ScrollBar>(new ScrollBar(*this));
}

void ScrollBar::RestoreDefault() noexcept
{
	SetScrollContainerSize(0.5f);
	SetPositionRatio(0.f);
	m_bounded.Reset();
}

bool ScrollBar::Setup(const UILayout& layout,
	unique_ptr<PatchTextureStd3> scrollTrack,
	unique_ptr<TextureSwitcher> scrollButton)
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

bool ScrollBar::Setup(unique_ptr<PatchTextureStd3> scrollTrack, unique_ptr<TextureSwitcher> scrollButton)
{
	return Setup({}, move(scrollTrack), move(scrollButton));
}

bool ScrollBar::ImplementBindSourceInfo(TextureResourceBinder*, ITextureController*) noexcept
{
	if (GetSize() == XMUINT2{})
		SetSize(UIEx(this).GetChildrenBoundsSize());

	SetScrollContainerSize(0.5f);	//기본값
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
	const XMUINT2& containerSize = m_scrollButton->GetSize();
	return static_cast<ReturnType>(trackSize.y - containerSize.y);
}

void ScrollBar::OnPressCB(KeyState keyState)
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

//이 함수는 세로만 적용돼 있다.
bool ScrollBar::ImplementChangeSize(const XMUINT2& newSize, bool isForce) noexcept
{
	float sizeRatio = static_cast<float>(newSize.y) / static_cast<float>(m_scrollTrack->GetSize().y);
	ReturnIfFalse(m_scrollTrack->ChangeSize(newSize, isForce));

	//크기가 바뀌면 상대적으로 버튼 크기가 정해지기 때문에 조정되어야 한다.
	uint32_t btnHeight = static_cast<uint32_t>(static_cast<float>(m_scrollButton->GetSize().y) * sizeRatio);
	auto btnSize = XMUINT2{ newSize.x, btnHeight };
	m_scrollButton->ChangeSize(btnSize, isForce);

	return true;
}

bool ScrollBar::UpdateScrollView(uint32_t viewArea, uint32_t contentSize) noexcept
{
	if(GetToolMode())
	{
		SetScrollContainerSize(0.5f);
		return false;
	}

	if (contentSize <= viewArea)
		return SetStateFlag(StateFlag::Active, false);

	bool changeFlag = SetStateFlag(StateFlag::Active, true);
	if (int height = viewArea - contentSize; height < 0)
		m_bounded.SetBounds(height, 0, 15);

	SetScrollContainerSize(static_cast<float>(viewArea) / static_cast<float>(contentSize));
	return changeFlag;
}

void ScrollBar::SetScrollContainerSize(float ratio) noexcept
{
	DirectionType dirType = m_scrollTrack->GetDirectionType();
	const auto& area = m_scrollTrack->GetArea();
	XMUINT2 ratioSize = GetSizeFromRectangle(area);

	switch (dirType) {
	case DirectionType::Horizontal: ratioSize.x = static_cast<uint32_t>(area.width * ratio); break;
	case DirectionType::Vertical: ratioSize.y = static_cast<uint32_t>(area.height * ratio); break;
	}

	m_scrollButton->ChangeSize(ratioSize);
}

void ScrollBar::SetPositionRatio(float positionRatio) noexcept
{
	positionRatio = clamp(positionRatio, 0.f, 1.f);

	auto relativePosY = static_cast<int32_t>(GetMaxScrollRange<float>() * positionRatio);
	const auto& curPos = m_scrollButton->GetRelativePosition();
	m_scrollButton->SetRelativePosition({ curPos.x, relativePosY });
}

void ScrollBar::SetEnableWheel(bool enable) noexcept
{
	if(!m_isWheelEnabled && enable) ResetMouseWheelValue();
	m_isWheelEnabled = enable;
}