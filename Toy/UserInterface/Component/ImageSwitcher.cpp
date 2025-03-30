#include "pch.h"
#include "ImageSwitcher.h"
#include "ImageGrid.h"
#include "ImageGrid1.h"
#include "ImageGrid3.h"
#include "../TextureSourceBinder/TextureSourceBinder.h"
#include "../../../Include/IRenderer.h"
#include "../../InputManager.h"
#include "../JsonOperation.h"
#include "../../Utility.h"
#include "../../Config.h"

using enum InteractState;

ImageSwitcher::~ImageSwitcher() = default;
ImageSwitcher::ImageSwitcher() :
	m_image{ nullptr }
{}

ImageSwitcher::ImageSwitcher(const ImageSwitcher& o) :
	UIComponent{ o },
	m_stateKeys{ o.m_stateKeys },
	m_behaviorMode{ o.m_behaviorMode },
	m_sources{ o.m_sources },
	m_indexes{ o.m_indexes },
	m_image{ nullptr },
	m_state{ o.m_state }
{
	ReloadDatas();
}

void ImageSwitcher::ReloadDatas() noexcept
{
	vector<UIComponent*> componentList = GetChildComponents();
	m_image = static_cast<ImageGrid*>(componentList[0]);
}

unique_ptr<UIComponent> ImageSwitcher::CreateClone() const
{
	return unique_ptr<ImageSwitcher>(new ImageSwitcher(*this));
}

bool ImageSwitcher::operator==(const UIComponent& rhs) const noexcept
{
	ReturnIfFalse(UIComponent::operator==(rhs));
	const ImageSwitcher* o = static_cast<const ImageSwitcher*>(&rhs);

	return tie(m_stateKeys, m_behaviorMode) == tie(o->m_stateKeys, o->m_behaviorMode);
}

static unique_ptr<ImageGrid> CreateImageGrid(const UILayout& layout, ImagePart imgPart, const string& bindKey)
{
	UILayout ltLayout({ layout.GetSize(), Origin::LeftTop});
	switch (imgPart)	{
	case ImagePart::One: return CreateImageGrid1(ltLayout, bindKey);
	case ImagePart::ThreeH: return CreateImageGrid3(DirectionType::Horizontal, ltLayout, bindKey);
	case ImagePart::ThreeV: return CreateImageGrid3(DirectionType::Vertical, ltLayout, bindKey);
	default: return nullptr;
	}
}

bool ImageSwitcher::Setup(const UILayout& layout, ImagePart imgPart,
	const map<InteractState, string>& stateKeys, BehaviorMode behaviorMode)
{
	SetLayout(layout);
	unique_ptr<ImageGrid> img = nullptr;
	tie(img, m_image) = GetPtrs(CreateImageGrid(layout, imgPart, stateKeys.at(InteractState::Normal)));
	UIEx(this).AttachComponent(move(img), {});
	m_stateKeys = stateKeys;
	m_behaviorMode = behaviorMode;

	return true;
}

void ImageSwitcher::ClearInteraction() noexcept
{
	if (m_state && m_state == InteractState::Hovered)
		SetState(InteractState::Normal);
}

bool ImageSwitcher::ImplementBindSourceInfo(TextureSourceBinder* sourceBinder, ITextureController*) noexcept
{
	for (const auto& pair : m_stateKeys)
	{
		auto sourceInfoRef = sourceBinder->GetTextureSourceInfo(pair.second);
		ReturnIfFalse(sourceInfoRef);

		const auto& srcInfo = sourceInfoRef->get();
		auto curIndex = srcInfo.GetIndex();
		ReturnIfFalse(curIndex);

		m_indexes.emplace(pair.first, *curIndex);
		m_sources.emplace(pair.first, ImageSource{ srcInfo.filename, srcInfo.sources });
	}
	SetState(Normal);

	return true;
}

void ImageSwitcher::NormalMode(bool isPressed, bool isHeld) noexcept
{
	if (!Contains(GetArea(), InputManager::GetMouse().GetPosition()))
	{
		SetState(Normal);
		return;
	}

	SetState((isPressed || (m_state == Pressed && isHeld)) ? Pressed : Hovered);
}

//마우스 왼쪽키가 계속 눌러지고 있으면 영역을 벗어나도 눌러지는 state가 유지된다.(scrollbar에서 사용)
void ImageSwitcher::HoldToKeepPressedMode(bool isPressed, bool isHeld) noexcept
{
	if (m_state == Pressed && isHeld) //Pressed가 계속 되고 있다면 리턴한다.
	{
		m_onPressCB(KeyState::Held);
		return;
	}

	NormalMode(isPressed, isHeld);

	if (m_state == Pressed && isPressed) m_onPressCB(KeyState::Pressed);
}

bool ImageSwitcher::ImplementUpdate(const DX::StepTimer&) noexcept
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

bool ImageSwitcher::ImplementChangeSize(const XMUINT2& size) noexcept
{
	ReturnIfFalse(ranges::all_of(GetChildComponents(), [&size](const auto& component) {
		return component->ChangeSize(size);
		}));
	return UIComponent::ImplementChangeSize(size);
}

void ImageSwitcher::SetState(InteractState state) noexcept
{
	if (m_state == state) return;

	m_image->SetIndexedSource(m_indexes[state], m_sources[state].list);
	m_state = state;
}

void ImageSwitcher::SerializeIO(JsonOperation& operation)
{
	UIComponent::SerializeIO(operation);
	operation.Process("StateKey", m_stateKeys);
	operation.Process("BehaviorMode", m_behaviorMode);

	if (operation.IsWrite()) return;
	ReloadDatas();
}

unique_ptr<ImageSwitcher> CreateImageSwitcher(const UILayout& layout, ImagePart imgPart,
	const map<InteractState, string>& stateKeys, BehaviorMode behaviorMode)
{
	unique_ptr<ImageSwitcher> switcher = make_unique<ImageSwitcher>();
	return CreateIfSetup(move(switcher), layout, imgPart, stateKeys, behaviorMode);
}