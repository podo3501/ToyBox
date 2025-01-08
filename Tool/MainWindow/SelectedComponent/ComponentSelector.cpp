#include "pch.h"
#include "ComponentSelector.h"
#include "ComponentTooltip.h"
#include "ComponentEdit/EditImageGrid.h"
#include "../Toy/InputManager.h"
#include "../Toy/HelperClass.h"
#include "../Toy/Utility.h"
#include "../Toy/UserInterface/UIComponent.h"
#include "../../Utility.h"
#include "../Toy/UserInterface/Panel.h"
#include "../Toy/UserInterface/ImageGrid1.h"
#include "../Toy/UserInterface/ImageGrid3.h"
#include "../Toy/UserInterface/ImageGrid9.h"

ComponentSelector::~ComponentSelector() = default;
ComponentSelector::ComponentSelector(IRenderer* renderer, UIComponent* panel) :
	m_renderer{ renderer },
	m_mainWnd{ nullptr },
	m_tooltip{ make_unique<ComponentTooltip>(panel) },
	m_editWindow{ nullptr },
	m_panel{ panel },
	m_component{ nullptr }
{}

void ComponentSelector::SetPanel(UIComponent* panel) noexcept
{
	m_panel = panel;
	m_tooltip->SetPanel(panel);
}

template <typename EditType, typename ComponentType, typename... Args>
unique_ptr<EditWindow> CreateEdit(UIComponent* component, Args&&... args) 
{
	return make_unique<EditType>(ComponentCast<ComponentType>(component), forward<Args>(args)...);
}

//이게 점점 커지면 include도 많이 생기고 해서 static factory클래스로 만들어야 할 것 같다.
static unique_ptr<EditWindow> CreateEditWindow(UIComponent* component, IRenderer* renderer)
{
	if (!component) return nullptr;

	const string& type = component->GetType();
	if (type == "class Panel") return CreateEdit<EditPanel, Panel*>(component);
	if (type == "class ImageGrid1") return CreateEdit<EditImageGrid1, ImageGrid1*>(component, renderer);
	if (type == "class ImageGrid3") return CreateEdit<EditImageGrid3, ImageGrid3*>(component, renderer);
	if (type == "class ImageGrid9") return CreateEdit<EditImageGrid9, ImageGrid9*>(component, renderer);

	return nullptr;
}

void ComponentSelector::SetComponent(UIComponent* component) noexcept
{
	if (m_component == nullptr && component == nullptr) return;
	if (m_component == component)
	{
		SetComponent(nullptr);
		return;
	}

	m_editWindow = CreateEditWindow(component, m_renderer);

	m_component = component;
	m_tooltip->SetComponent(component);
}

void ComponentSelector::SelectComponent(InputManager* inputManager) noexcept
{
	const auto mouseTracker = inputManager->GetMouse();
	if (mouseTracker->leftButton != Mouse::ButtonStateTracker::PRESSED) return;

	static vector<UIComponent*> preComponentList{ nullptr };
	vector<UIComponent*> componentList;
	const XMINT2& pos = mouseTracker->GetOffsetPosition();
	m_panel->GetComponents(pos, componentList);
	if (componentList.empty()) return;

	if (preComponentList == componentList)
		RepeatedSelection(componentList);
	else //마우스가 다른 컴포넌트를 선택해서 리스트가 바뀌었다면 제일 첫번째 원소를 찍어준다.
	{
		SetComponent(componentList.back());
		preComponentList = move(componentList);
	}
}

void ComponentSelector::Update(InputManager* inputManager, bool bPopupActive) noexcept
{
	auto pressedKey = inputManager->GetKeyboard()->pressed;
	if (pressedKey.Escape)
	{
		SetComponent(nullptr);
		return;
	}

	if (m_editWindow)
		m_editWindow->Update(inputManager);

	if (!bPopupActive && IsWindowFocus(m_mainWnd))	//팝업이 활동중이면
		SelectComponent(inputManager);
}

void ComponentSelector::Render(bool bPopupActive)
{
	if(!bPopupActive && IsWindowFocus(m_mainWnd)) //component를 새로 생성시켜서 붙일려고 한다면 툴팁은 보이지 않게 한다.
		m_tooltip->Render(m_mainWnd);

	if(m_component)
		DrawRectangle(m_component->GetRectangle(), m_mainWnd);

	if (m_editWindow)
		m_editWindow->Render(m_mainWnd);
}

void ComponentSelector::RepeatedSelection(const vector<UIComponent*>& componentList) noexcept
{
	auto findIdx = FindIndex(componentList, m_component);
	if (!findIdx.has_value())
	{
		SetComponent(componentList.back());
		return;
	}

	int idx = findIdx.value() - 1;
	if (idx < 0) idx = static_cast<int>(componentList.size() - 1);
	SetComponent(componentList[idx]);
}

