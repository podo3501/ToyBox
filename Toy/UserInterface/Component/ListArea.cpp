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
	m_prototypeContainer->SetStateFlag(StateFlag::Active, false); //Prototype를 만드는 컨테이너이기 때문에 비활동적으로 셋팅한다.
	UIEx(this).AttachComponent(move(container), {});

	auto areaController = make_unique<Panel>("AreaController", layout);
	areaController->EnableChildMouseEvents(true);
	m_areaController = areaController.get();
	UIEx(this).AttachComponent(move(areaController), {});

	//BackGround, RenderTarget은 순서가 중요하다. BG뒤에 RT가 와야 이미지가 덮히지 않는다.
	m_bgImage = bgImage.get();
	m_bgImage->Rename("Background Image");
	UIEx(this).AttachComponent(move(bgImage), {});

	auto renderTex = CreateRenderTexture(layout, true, m_areaController);
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
	UIEx(m_areaController).AttachComponent(move(cloneContainer), {});

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
	if (!m_areaController->IsMouseInArea()) return true;

	static WheelBoundedValue wheelBoundedValue(-50, 0, 8);
	if (m_areaController->OnEnterArea())
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