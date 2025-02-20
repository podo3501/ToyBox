#include "pch.h"
#include "ListArea.h"
#include "../JsonOperation.h"
#include "RenderTexture.h"

ListArea::~ListArea() = default;
ListArea::ListArea() noexcept :
	m_bgImage{ nullptr },
	m_prototypeContainer{ nullptr }
{}

ListArea::ListArea(const ListArea& o) noexcept :
	UIComponent{ o }
{
	ReloadDatas();
}

void ListArea::ReloadDatas() noexcept
{
	vector<UIComponent*> componentList = GetChildComponents();
	m_bgImage = componentList[0];
	m_prototypeContainer = componentList[1];
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

	auto renderTex = CreateRenderTexture(layout, true, bgImage.get());
	m_renderTex = renderTex.get();
	UIEx(this).AttachComponent(move(renderTex), {});

	m_bgImage = bgImage.get();
	m_bgImage->Rename("Containers");
	UIEx(this).AttachComponent(move(bgImage), {});
	
	m_prototypeContainer = container.get();
	m_prototypeContainer->Rename("PrototypeContainer");
	m_prototypeContainer->SetStateFlag(StateFlag::Active, false); //Prototype�� ����� �����̳��̱� ������ ��Ȱ�������� �����Ѵ�.
	UIEx(this).AttachComponent(move(container), {});

	//�ڽĵ��� attach detach�� �Ǵµ� prototype�� �ڽ������� detach�� �� �Ǿ�� �Ѵ�. �����ؾ���

	return true;
}

UIComponent* ListArea::PrepareContainer()
{
	static int32_t y = 0;
	auto cloneContainer = m_prototypeContainer->Clone();
	auto cloneContainerPtr = cloneContainer.get();
	UIEx(m_bgImage).AttachComponent(move(cloneContainer), {});

	cloneContainerPtr->SetStateFlag(StateFlag::Active, true);
	cloneContainerPtr->SetRelativePosition({ 0, y });
	y += cloneContainerPtr->GetSize().y;

	return cloneContainerPtr;
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