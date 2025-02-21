#include "pch.h"
#include "ListArea.h"
#include "../JsonOperation.h"
#include "RenderTexture.h"
#include "Panel.h"
#include "../../InputManager.h"

ListArea::~ListArea() = default;
ListArea::ListArea() noexcept :
	m_bgImage{ nullptr },
	m_renderTex{ nullptr },
	m_prototypeContainer{ nullptr },
	m_areaController{ nullptr }
{}

ListArea::ListArea(const ListArea& o) noexcept :
	UIComponent{ o },
	m_bgImage{ nullptr },
	m_renderTex{ nullptr },
	m_prototypeContainer{ nullptr },
	m_areaController{ nullptr }
{
	ReloadDatas();
}

void ListArea::ReloadDatas() noexcept
{
	//vector<UIComponent*> componentList = GetChildComponents();
	//m_bgImage = componentList[0];
	//m_prototypeContainer = componentList[1];
}

unique_ptr<UIComponent> ListArea::CreateClone() const
{
	return unique_ptr<ListArea>(new ListArea(*this));
}

void ListArea::ChangeSize(const XMUINT2& size) noexcept
{
	m_prototypeContainer->ChangeSize(size);
	UIComponent::ChangeSize(size);
}

bool ListArea::Setup(const UILayout& layout, unique_ptr<UIComponent>&& bgImage, unique_ptr<UIComponent>&& container) noexcept
{
	SetLayout(layout);
	
	m_prototypeContainer = container.get();
	m_prototypeContainer->Rename("Prototype Container");
	m_prototypeContainer->SetStateFlag(StateFlag::Active, false); //Prototype�� ����� �����̳��̱� ������ ��Ȱ�������� �����Ѵ�.
	UIEx(this).AttachComponent(move(container), {});

	auto areaController = make_unique<Panel>("AreaController", layout);
	areaController->EnableChildMouseEvents(true);
	m_areaController = areaController.get();
	UIEx(this).AttachComponent(move(areaController), {});

	//BackGround, RenderTarget�� ������ �߿��ϴ�. BG�ڿ� RT�� �;� �̹����� ������ �ʴ´�.
	m_bgImage = bgImage.get();
	m_bgImage->Rename("Background Image");
	UIEx(this).AttachComponent(move(bgImage), {});

	auto renderTex = CreateRenderTexture(layout, true, m_areaController);
	m_renderTex = renderTex.get();
	UIEx(this).AttachComponent(move(renderTex), {});
	
	//�ڽĵ��� attach detach�� �Ǵµ� prototype�� �ڽ������� detach�� �� �Ǿ�� �Ѵ�. �����ʿ�

	return true;
}

UIComponent* ListArea::PrepareContainer()
{
	static int32_t y = 0;
	auto cloneContainer = m_prototypeContainer->Clone();
	auto cloneContainerPtr = cloneContainer.get();
	UIEx(m_areaController).AttachComponent(move(cloneContainer), {});

	cloneContainerPtr->SetStateFlag(StateFlag::Active, true);
	cloneContainerPtr->SetRelativePosition({ 0, y });
	y += cloneContainerPtr->GetSize().y;

	m_containers.emplace_back(cloneContainerPtr);

	return cloneContainerPtr;
}

bool ListArea::ImplementUpdatePosition(const XMINT2&) noexcept
{
	if (!m_areaController->IsMouseInArea()) return true;

	if (m_areaController->OnEnterArea()) 
		ResetMouseWeelValue();

	static int wheelPos{ 0 };
	static int lastWheelPos{ 0 };
	if (auto value = GetMouseWheelValue(); value)
	{
		wheelPos += (value * 8);
		wheelPos = clamp(wheelPos, -50, 0);
		int gap = wheelPos - lastWheelPos;
		lastWheelPos = wheelPos;

		for (auto container : m_containers)
		{
			auto pos = container->GetRelativePosition();
			pos->y += gap;
			container->SetRelativePosition(*pos);
		}
	}

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