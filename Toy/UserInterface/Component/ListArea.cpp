#include "pch.h"
#include "ListArea.h"
#include "../JsonOperation.h"
#include "RenderTexture.h"
#include "../../InputManager.h"
#include "../../Utility.h"
#include "Container.h"
#include "../../StepTimer.h"

ListArea::~ListArea() = default;
ListArea::ListArea() noexcept :
	m_prototypeContainer{ nullptr },
	m_bgImage{ nullptr },
	m_renderTex{ nullptr }	
{}

ListArea::ListArea(const ListArea& o) noexcept :
	UIComponent{ o },
	m_prototypeContainer{ nullptr },
	m_bgImage{ nullptr },
	m_renderTex{ nullptr }
{
	ReloadDatas();
}

void ListArea::ReloadDatas() noexcept
{
	vector<UIComponent*> componentList = GetChildComponents();
	m_prototypeContainer = componentList[0];
	m_renderTex = ComponentCast<RenderTexture*>(componentList[1]);
	m_bgImage = m_renderTex->GetRenderedComponent();
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

	return true;
}

void ListArea::ChangeSize(const XMUINT2& size) noexcept
{
	m_prototypeContainer->ChangeSize(size);
	UIComponent::ChangeSize(size);
}

bool ListArea::Setup(const UILayout& layout, unique_ptr<UIComponent>&& bgImage, unique_ptr<UIComponent>&& container) noexcept
{
	SetLayout(layout);
	UILayout partLayout{ layout.GetSize(), Origin::LeftTop }; //�Ӽ����� �������� �ʴ´�.
	
	m_prototypeContainer = container.get();
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
		
	//�ڽĵ��� attach detach�� �Ǵµ� prototype�� �ڽ������� detach�� �� �Ǿ�� �Ѵ�. �����ʿ�

	return true;
}

UIComponent* ListArea::PrepareContainer()
{
	static int32_t y = 0; //�ӽ÷� �۾��Ѱ� �̰� �����ؾ���. ?!?
	auto cloneContainer = m_prototypeContainer->Clone();
	auto cloneContainerPtr = cloneContainer.get();
	UIEx(m_bgImage).AttachComponent(move(cloneContainer), {});

	cloneContainerPtr->SetStateFlag(StateFlag::Active, true);
	cloneContainerPtr->SetRelativePosition({ 0, y });
	y += cloneContainerPtr->GetSize().y;

	m_containers.emplace_back(cloneContainerPtr);
	if(int height = m_renderTex->GetSize().y - y; height < 0)
		m_bounded.SetBounds(height, 0, 15);

	return cloneContainerPtr;
}

void ListArea::ScrollContainers(const DX::StepTimer& timer) noexcept
{
	if (m_renderTex->OnEnterArea())
		ResetMouseWheelValue();

	int wheelValue{ 0 };
	if (m_renderTex->IsMouseInArea())
		wheelValue = GetMouseWheelValue();

	auto value = m_bounded.GetValue(wheelValue, timer);
	if (!value) return;
	
	for (auto container : m_containers)
	{
		auto pos = container->GetRelativePosition();
		pos->y += value;
		container->SetRelativePosition(*pos);
	}
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

bool ListArea::ImplementUpdatePosition(const DX::StepTimer& timer, const XMINT2&) noexcept
{
	CheckMouseInteraction();
	ScrollContainers(timer);

	return true;
}

void ListArea::SerializeIO(JsonOperation& operation)
{
	UIComponent::SerializeIO(operation);

	if (operation.IsWrite()) return;
	ReloadDatas();
}

unique_ptr<ListArea> CreateListArea(const UILayout& layout, 
	unique_ptr<UIComponent>&& bgImage,
	unique_ptr<UIComponent>&& container)
{
	unique_ptr<ListArea> listArea= make_unique<ListArea>();
	if (!listArea->Setup(layout, move(bgImage), move(container))) return nullptr;
	return listArea;
}