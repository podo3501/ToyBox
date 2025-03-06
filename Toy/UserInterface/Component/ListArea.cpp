#include "pch.h"
#include "ListArea.h"
#include "../JsonOperation.h"
#include "RenderTexture.h"
#include "ScrollBar.h"
#include "ScrollSlider.h"
#include "Container.h"
#include "../../InputManager.h"
#include "../../Utility.h"
#include "Container.h"
#include "../../StepTimer.h"

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
	m_prototypeContainer = ComponentCast<Container*>(componentList[0]);
	m_renderTex = ComponentCast<RenderTexture*>(componentList[1]);
	m_bgImage = m_renderTex->GetRenderedComponent();
	m_scrollBar = ComponentCast<ScrollBar*>(componentList[2]);
	m_scrollSlider = m_scrollBar->GetScrollSlider();
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

bool ListArea::ImplementChangeSize(const XMUINT2& size) noexcept
{
	ReturnIfFalse(m_prototypeContainer->ChangeSize(size));
	return UIComponent::ImplementChangeSize(size);
}

bool ListArea::Setup(const UILayout& layout, unique_ptr<UIComponent> bgImage, 
	unique_ptr<UIComponent> container, unique_ptr<UIComponent> scrollBar) noexcept
{
	SetLayout(layout);
	UILayout partLayout{ layout.GetSize(), Origin::LeftTop }; //�Ӽ����� �������� �ʴ´�.
	
	m_prototypeContainer = ComponentCast<Container*>(container.get());
	m_prototypeContainer->ChangeSize({ layout.GetSize().x - 22, 30 });
	m_prototypeContainer->Rename("Prototype Container");
	m_prototypeContainer->SetStateFlag(StateFlag::Active, false); //Prototype�� ����� �����̳��̱� ������ ��Ȱ�������� �����Ѵ�.
	UIEx(this).AttachComponent(move(container), {});

	// ���Ŀ� �پ��� ����(2��¥�� ListArea ����)�� ������ �� bgImageó�� ���ο� ������Ʈ�� ���� �ִ´�.
	// ������ Background�� �����ϰ� ������ �پ��� ����Ʈ ���°� ���ü� �ִ�.
	// RenderTexture�� �ܼ��� RenderTexture�� �ϴ� ���ҷ� �� ����.
	m_bgImage = bgImage.get();
	m_bgImage->Rename("Background Image");

	auto renderTex = CreateRenderTexture(partLayout, move(bgImage));
	renderTex->EnableChildMouseEvents(true);
	m_renderTex = renderTex.get();
	UIEx(this).AttachComponent(move(renderTex), {});

	m_scrollBar = ComponentCast<ScrollBar*>(scrollBar.get());
	m_scrollBar->SetStateFlag(StateFlag::Active, false);
	m_scrollBar->ChangeSize({ 22, layout.GetSize().y });
	XMINT2 scrollBarPos{ static_cast<int32_t>(layout.GetSize().x - m_scrollBar->GetSize().x), 0 };
	UIEx(this).AttachComponent(move(scrollBar), scrollBarPos);

	m_scrollSlider = m_scrollBar->GetScrollSlider();
	m_scrollSlider->AddScrollChangedCB([this](float ratio) { OnScrollChangedCB(ratio); });
		
	//�ڽĵ��� attach detach�� �Ǵµ� prototype�� �ڽ������� detach�� �� �Ǿ�� �Ѵ�. �����ʿ�

	return true;
}

int32_t ListArea::GetContainerHeight() const noexcept
{
	int32_t height{ 0 };
	for (auto container : m_containers)
		height += container->GetSize().y;

	return height;
}

UIComponent* ListArea::PrepareContainer()
{
	auto cloneContainer = m_prototypeContainer->Clone();
	auto cloneContainerPtr = cloneContainer.get();
	UIEx(m_bgImage).AttachComponent(move(cloneContainer), {});

	const auto& containerHeight = GetContainerHeight();
	cloneContainerPtr->SetStateFlag(StateFlag::Active, true);
	cloneContainerPtr->SetRelativePosition({ 0, containerHeight });
	m_containers.emplace_back(cloneContainerPtr);

	auto viewArea = m_renderTex->GetSize().y;
	auto curHeight = containerHeight + cloneContainerPtr->GetSize().y;

	float contentRatio = static_cast<float>(viewArea) / static_cast<float>(curHeight);
	m_scrollBar->SetStateFlag(StateFlag::Active, (contentRatio) < 1.f ? true : false);
	m_scrollSlider->SetViewContent(viewArea, curHeight);

	return cloneContainerPtr;
}

void ListArea::ClearContainers() noexcept
{
	m_containers.clear();
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
		if (Container* cur = ComponentCast<Container*>(container); cur)
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
	unique_ptr<UIComponent> container,
	unique_ptr<UIComponent> scrollBar)
{
	unique_ptr<ListArea> listArea= make_unique<ListArea>();
	if (!listArea->Setup(layout, move(bgImage), move(container), move(scrollBar))) return nullptr;
	return listArea;
}