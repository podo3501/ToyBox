#include "pch.h"
#include "ListArea.h"
#include "../JsonOperation.h"
#include "RenderTexture.h"
#include "../../InputManager.h"

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
	m_bgImage = componentList[1];
	m_renderTex = ComponentCast<RenderTexture*>(componentList[2]);
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
	UILayout partLayout{ layout.GetSize(), Origin::LeftTop }; //속성들은 정렬하지 않는다.
	
	m_prototypeContainer = container.get();
	m_prototypeContainer->Rename("Prototype Container");
	m_prototypeContainer->SetStateFlag(StateFlag::Active, false); //Prototype를 만드는 컨테이너이기 때문에 비활동적으로 셋팅한다.
	UIEx(this).AttachComponent(move(container), {});

	// 추후에 다양한 형태(2줄짜리 ListArea 같은)가 나오면 이 bgImage처럼 새로운 컴포넌트를 만들어서 넣는다.
	// 지금은 Background로 간단하게 했지만 다양한 리스트 형태가 나올수 있다.
	// RenderTexture는 단순히 RenderTexture만 하는 역할로 놔 두자.
	m_bgImage = bgImage.get();
	m_bgImage->Rename("Background Image");

	auto renderTex = CreateRenderTexture(partLayout, move(bgImage));
	renderTex->EnableChildMouseEvents(true);
	m_renderTex = renderTex.get();
	UIEx(this).AttachComponent(move(renderTex), {});
		
	//자식들은 attach detach가 되는데 prototype은 자식이지만 detach가 안 되어야 한다. 셋팅필요

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

	m_containers.emplace_back(cloneContainerPtr);

	return cloneContainerPtr;
}

class WheelBoundedValue
{
public:
	WheelBoundedValue() = delete;
	WheelBoundedValue(int min, int max, int unit) : 
		m_min{ min }, m_max{ max }, m_unit{ unit }
	{}

	int GetValue() noexcept
	{
		return ValidateRange(GetMouseWheelValue() * m_unit);
	}

	void ResetWheelValue() const noexcept
	{
		ResetMouseWheelValue();
	}

private:
	int ValidateRange(int value) noexcept
	{
		m_value += value;
		m_value = clamp(m_value, m_min, m_max);
		int gap = m_value - m_lastValue;
		m_lastValue = m_value;

		return gap;
	}
	
	int m_min{ 0 };
	int m_max{ 0 };
	int m_unit{ 0 };
	int m_value{ 0 };
	int m_lastValue{ 0 };
};

bool ListArea::ImplementUpdatePosition(const DX::StepTimer&, const XMINT2&) noexcept
{
	if (!m_renderTex->IsMouseInArea()) return true;

	static WheelBoundedValue wheelBoundedValue(-50, 0, 8);
	if (m_renderTex->OnEnterArea())
		wheelBoundedValue.ResetWheelValue();
		
	if(auto value = wheelBoundedValue.GetValue(); value)
	{
		for (auto container : m_containers)
		{
			auto pos = container->GetRelativePosition();
			pos->y += value;
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