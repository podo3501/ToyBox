#include "pch.h"
#include "ListArea.h"
#include "../../JsonOperation/JsonOperation.h"
#include "RenderTexture.h"
#include "ScrollBar.h"
#include "ScrollSlider.h"
#include "TextureSwitcher.h"
#include "InputManager.h"
#include "Utility.h"
#include "StepTimer.h"

ListArea::~ListArea() = default;
ListArea::ListArea() noexcept :
	m_prototypeContainer{ nullptr },
	m_bgImage{ nullptr },
	m_renderTex{ nullptr },
	m_scrollBar{ nullptr },
	m_scrollSlider{ nullptr }
{}

ListArea::ListArea(const ListArea& o) noexcept :
	UIComponent{ o },
	m_prototypeContainer{ nullptr },
	m_bgImage{ nullptr },
	m_renderTex{ nullptr },
	m_scrollBar{ nullptr },
	m_scrollSlider{ nullptr }
{
	ReloadDatas();
}

void ListArea::ReloadDatas() noexcept
{
	vector<UIComponent*> componentList = GetChildComponents();
	m_prototypeContainer = ComponentCast<TextureSwitcher*>(componentList[0]);
	m_renderTex = ComponentCast<RenderTexture*>(componentList[1]);
	m_bgImage = m_renderTex->GetRenderedComponent();
	m_scrollBar = ComponentCast<ScrollBar*>(componentList[2]);
	m_scrollSlider = m_scrollBar->GetScrollSlider();
	m_scrollSlider->AddScrollChangedCB([this](float ratio) { OnScrollChangedCB(ratio); });
}

unique_ptr<UIComponent> ListArea::CreateClone() const
{
	return unique_ptr<ListArea>(new ListArea(*this));
}

bool ListArea::operator==(const UIComponent& rhs) const noexcept
{
	ReturnIfFalse(UIComponent::operator==(rhs));

	const ListArea* o = static_cast<const ListArea*>(&rhs);
	ReturnIfFalse(EqualComponent(m_prototypeContainer, o->m_prototypeContainer));
	ReturnIfFalse(EqualComponent(m_bgImage, o->m_bgImage));
	ReturnIfFalse(EqualComponent(m_renderTex, o->m_renderTex));
	ReturnIfFalse(EqualComponent(m_scrollBar, o->m_scrollBar));
	ReturnIfFalse(EqualComponent(m_scrollSlider, o->m_scrollSlider));

	return true;
}

bool ListArea::Setup(const UILayout& layout, unique_ptr<UIComponent> bgImage,
	unique_ptr<TextureSwitcher> switcher, unique_ptr<ScrollBar> scrollBar) noexcept
{
	SetLayout(layout);
	UILayout partLayout{ layout.GetSize(), Origin::LeftTop }; //속성들은 정렬하지 않는다.

	m_prototypeContainer = switcher.get();
	m_prototypeContainer->Rename("Prototype Container");
	m_prototypeContainer->SetStateFlag(StateFlag::ActiveUpdate | StateFlag::Render, false); //Prototype를 만드는 컨테이너이기 때문에 비활동적으로 셋팅한다.
	m_prototypeContainer->SetStateFlag(StateFlag::RenderEditable, true);
	UIEx(this).AttachComponent(move(switcher), {});

	// 추후에 다양한 형태(2줄짜리 ListArea 같은)가 나오면 이 bgImage처럼 새로운 컴포넌트를 만들어서 넣는다.
	// 지금은 Background로 간단하게 했지만 다양한 리스트 형태가 나올수 있다.
	// RenderTexture는 단순히 RenderTexture만 하는 역할로 놔 두자.
	m_bgImage = bgImage.get();
	m_bgImage->Rename("Background Image");

	auto renderTex = CreateRenderTexture(partLayout, move(bgImage));
	renderTex->EnableChildMouseEvents(true);
	m_renderTex = renderTex.get();
	UIEx(this).AttachComponent(move(renderTex), {});

	m_scrollBar = scrollBar.get();
	m_scrollBar->ChangeOrigin(Origin::RightTop);
	m_scrollBar->SetStateFlag(StateFlag::Render, false);
	m_scrollBar->SetStateFlag(StateFlag::RenderEditable, true);
	UIEx(this).AttachComponent(move(scrollBar), { static_cast<int32_t>(layout.GetSize().x), 0 });

	m_scrollSlider = m_scrollBar->GetScrollSlider();
	m_scrollSlider->AddScrollChangedCB([this](float ratio) { OnScrollChangedCB(ratio); });
		
	//자식들은 attach detach가 되는데 prototype은 자식이지만 detach가 안 되어야 한다. 셋팅필요

	return true;
}

bool ListArea::ImplementBindSourceInfo(TextureResourceBinder*, ITextureController*) noexcept
{
	ChangeSizeY(m_scrollBar, GetSize());
	return true;
}

bool ListArea::ImplementChangeSize(const XMUINT2& size) noexcept
{
	ReturnIfFalse(ChangeSizeX(m_prototypeContainer, size));
	ReturnIfFalse(ChangeSizeY(m_scrollBar, size));
	ReturnIfFalse(m_renderTex->ChangeSize(size));
	UpdateScrollBar();

	return UIComponent::ImplementChangeSize(size);
}

int32_t ListArea::GetContainerHeight() const noexcept
{
	int32_t height{ 0 };
	for (auto container : m_containers)
		height += container->GetSize().y;

	return height;
}

void ListArea::UpdateScrollBar() noexcept
{
	bool isActiveChange = m_scrollBar->UpdateScrollView(m_renderTex->GetSize().y, GetContainerHeight());
	if (isActiveChange) ResizeContainerForScrollbar();
}

XMUINT2 ListArea::GetUsableContentSize() const noexcept
{
	uint32_t padding = m_scrollBar->HasStateFlag(StateFlag::Active) ? m_scrollBar->GetSize().x : 0;
	XMUINT2 usableSize{ m_prototypeContainer->GetSize() };
	usableSize.x -= padding;

	return usableSize;
}

void ListArea::ResizeContainerForScrollbar() noexcept
{
	XMUINT2 usableSize(GetUsableContentSize());
	
	for (auto container : m_containers)
		container->ChangeSize(usableSize);
}

UIComponent* ListArea::PrepareContainer()
{
	auto [cloneContainer, cloneContainerPtr] = GetPtrs(m_prototypeContainer->Clone());
	UIEx(m_bgImage).AttachComponent(move(cloneContainer), {});

	const auto& containerHeight = GetContainerHeight();
	cloneContainerPtr->SetStateFlag(StateFlag::Active, true);
	cloneContainerPtr->SetStateFlag(StateFlag::RenderEditable, false);
	cloneContainerPtr->SetRelativePosition({ 0, containerHeight });
	cloneContainerPtr->ChangeSize(GetUsableContentSize());
	m_containers.emplace_back(cloneContainerPtr);

	UpdateScrollBar();

	return cloneContainerPtr;
}

UIComponent* ListArea::GetContainer(unsigned int idx) const noexcept
{
	if (m_containers.size() <= idx) return nullptr;
	return m_containers[idx];
}

bool ListArea::RemoveContainer(unsigned int idx) noexcept
{
	auto container = GetContainer(idx);
	if (!container) return false;
	
	auto [detach, _] = UIEx(container).DetachComponent();
	if (!detach) return false;
	m_containers.erase(m_containers.begin() + idx);

	UpdateScrollBar();
	return true;
}

void ListArea::ClearContainers() noexcept
{
	for (auto container : m_containers)
		UIEx(container).DetachComponent();
	m_containers.clear();
	
	UpdateScrollBar();
}

void ListArea::MoveContainers(int32_t targetPos) noexcept
{
	const auto& containerHeight = m_prototypeContainer->GetSize().y;
	ranges::for_each(m_containers, [idx{ 0u }, targetPos, containerHeight](auto& container) mutable {
		XMINT2 pos = container->GetRelativePosition();
		pos.y = targetPos + ((idx++) * containerHeight);
		container->SetRelativePosition(pos);
		});
}

void ListArea::OnScrollChangedCB(float ratio)
{
	const auto& totalContainerHeight = GetContainerHeight();
	auto viewArea = m_renderTex->GetSize().y;
	auto startPos = -static_cast<int32_t>(ratio * (totalContainerHeight - viewArea));

	MoveContainers(startPos);
}

void ListArea::ScrollContainers(const DX::StepTimer&) noexcept
{
	m_scrollSlider->SetEnableWheel(m_renderTex->IsMouseInArea());
}

void ListArea::CheckMouseInteraction() noexcept
{
	if (!m_renderTex->OnLeaveArea()) return;

	for (auto& container : m_containers)
	{
		if (TextureSwitcher* cur = ComponentCast<TextureSwitcher*>(container); cur)
			cur->ClearInteraction();
	}
}

void ListArea::UpdateContainersScroll(const DX::StepTimer& timer) noexcept
{
	if (m_containers.empty()) return;

	CheckMouseInteraction();
	ScrollContainers(timer);
}

bool ListArea::ImplementUpdate(const DX::StepTimer& timer) noexcept
{
	UpdateContainersScroll(timer);

	return true;
}

void ListArea::SerializeIO(JsonOperation& operation)
{
	UIComponent::SerializeIO(operation);

	if (operation.IsWrite()) return;
	ReloadDatas();
}

unique_ptr<ListArea> CreateListArea(const UILayout& layout, 
	unique_ptr<UIComponent> bgImage,
	unique_ptr<TextureSwitcher> switcher,
	unique_ptr<ScrollBar> scrollBar)
{
	unique_ptr<ListArea> listArea= make_unique<ListArea>();
	return CreateIfSetup(move(listArea), layout, move(bgImage), move(switcher), move(scrollBar));
}