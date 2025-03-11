#include "pch.h"
#include "ImageSwitcher.h"
#include "ImageGrid.h"
#include "ImageGrid1.h"
#include "../../../Include/IRenderer.h"
#include "../../InputManager.h"
#include "../JsonOperation.h"
#include "../../Utility.h"
#include "../../Config.h"

using enum InteractState;

ImageSwitcher::~ImageSwitcher()
{
	Release();
}

ImageSwitcher::ImageSwitcher() :
	m_image{ nullptr },
	m_texController{ nullptr }
{}

void ImageSwitcher::Release() noexcept
{
	if (!m_texController) return;

	for (const auto& index : m_indexes)
		m_texController->ReleaseTexture(index.second);
	m_texController = nullptr;
	m_indexes.clear();
}

ImageSwitcher::ImageSwitcher(const ImageSwitcher& o) :
	UIComponent{ o },
	m_sources{ o.m_sources },
	m_behaviorMode{ o.m_behaviorMode },
	m_indexes{ o.m_indexes },
	m_image{ nullptr },
	m_texController{ o.m_texController }
{
	ReloadDatas();
}

void ImageSwitcher::ReloadDatas() noexcept
{
	vector<UIComponent*> componentList = GetChildComponents();
	m_image = static_cast<ImageGrid*>(componentList[0]);
}

void ImageSwitcher::AddRef() const noexcept
{
	if (!m_texController) return;

	for (const auto& index : m_indexes)
		m_texController->AddRef(index.second);
}

unique_ptr<UIComponent> ImageSwitcher::CreateClone() const
{
	auto clone = unique_ptr<ImageSwitcher>(new ImageSwitcher(*this));
	clone->AddRef();
	return clone;
}

bool ImageSwitcher::operator==(const UIComponent& rhs) const noexcept
{
	ReturnIfFalse(UIComponent::operator==(rhs));
	const ImageSwitcher* o = static_cast<const ImageSwitcher*>(&rhs);

	return tie(m_sources, m_behaviorMode) == tie(o->m_sources, o->m_behaviorMode);
}

bool ImageSwitcher::ImplementLoadResource(ITextureLoad* load)
{
	Release();

	size_t index{ 0 };
	for (const auto& source : m_sources)
	{
		const auto& imgSrc = source.second;
		ReturnIfFalse(load->LoadTexture(GetResourceFullFilename(imgSrc.filename), index, nullptr, nullptr));
		m_indexes.emplace(source.first, index);
	}
	
	return true;
}

bool ImageSwitcher::ImplementPostLoaded(ITextureController* texController)
{
	m_image->SetupFromData(GetSize(), m_indexes[Normal], m_sources[Normal].list);
	m_state = Normal;

	m_texController = texController;
	return true;
}

bool ImageSwitcher::Setup(const UILayout& layout, unique_ptr<ImageGrid> img,
	const map<InteractState, ImageSource>& sources, BehaviorMode behaviorMode) noexcept
{
	if (sources.empty()) return false;

	SetLayout(layout);
	m_image = img.get();
	UIEx(this).AttachComponent(move(img), {});
	m_sources = sources;
	m_behaviorMode = behaviorMode;

	return true;
}

void ImageSwitcher::NormalMode(bool isPressed, bool isHeld) noexcept
{
	if (!Contains(GetArea(), InputManager::GetMouse().GetPosition()))
	{
		SetState(Normal);
		return;
	}

	SetState((isPressed || (m_state == Pressed && isHeld)) ? Pressed : Hover);
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
	operation.Process("Sources", m_sources);
	operation.Process("BehaviorMode", m_behaviorMode);

	if (operation.IsWrite()) return;
	ReloadDatas();
}

unique_ptr<ImageSwitcher> CreateImageSwitcher(const UILayout& layout,
	unique_ptr<ImageGrid> img, const map<InteractState, ImageSource>& sources, BehaviorMode behaviorMode)
{
	return CreateIfSetup(make_unique<ImageSwitcher>(), layout, move(img), sources, behaviorMode);
}