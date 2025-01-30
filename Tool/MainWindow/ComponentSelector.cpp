#include "pch.h"
#include "ComponentSelector.h"
#include "SelectedComponent/ComponentTooltip.h"
#include "SelectedComponent/ComponentEdit/EditImageGrid.h"
#include "../Toy/InputManager.h"
#include "../Toy/Utility.h"
#include "../Dialog.h"
#include "../Utility.h"
#include "../Toy/UserInterface/Component/Panel.h"
#include "../Toy/UserInterface/Component/ImageGrid1.h"
#include "../Toy/UserInterface/Component/ImageGrid3.h"
#include "../Toy/UserInterface/Component/ImageGrid9.h"
#include "../Toy/UserInterface/UIComponentEx.h"
#include "../Toy/UserInterface/Command/CommandList.h"

ComponentSelector::~ComponentSelector() = default;
ComponentSelector::ComponentSelector(IRenderer* renderer, CommandList* cmdList, UIComponent* panel) :
	m_renderer{ renderer },
	m_cmdList{ cmdList },
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
static unique_ptr<EditWindow> CreateEditWindow(UIComponent* component, 
	IRenderer* renderer, ImGuiWindow* mainWnd, CommandList* cmdList)
{
	if (!component) return nullptr;

	ComponentID id = component->GetTypeID();
	if (id == ComponentID::Panel) return CreateEdit<EditPanel, Panel*>(component, mainWnd, cmdList);
	if (id == ComponentID::ImageGrid1) return CreateEdit<EditImageGrid1, ImageGrid1*>(component, renderer, mainWnd, cmdList);
	if (id == ComponentID::ImageGrid3) return CreateEdit<EditImageGrid3, ImageGrid3*>(component, renderer, mainWnd, cmdList);
	if (id == ComponentID::ImageGrid9) return CreateEdit<EditImageGrid9, ImageGrid9*>(component, renderer, mainWnd, cmdList);

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

	m_editWindow = CreateEditWindow(component, m_renderer, m_mainWnd, m_cmdList);

	m_component = component;
	m_tooltip->SetComponent(component);
}

void ComponentSelector::SelectComponent() noexcept
{
	if (!m_mainWnd) return;
	if (!IsInputAction(MouseButton::Left, KeyState::Pressed)) return;

	static vector<UIComponent*> preComponentList{ nullptr };
	vector<UIComponent*> componentList;
	const XMINT2& pos = GetWindowMousePos(m_mainWnd);
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

void ComponentSelector::Update() noexcept
{
	if (HandleEscapeKey()) return;
	if (UpdateEditWindow()) return;

	if (IsWindowFocus(m_mainWnd))
		SelectComponent();
}

bool ComponentSelector::HandleEscapeKey() noexcept
{
	if (!IsInputAction(Keyboard::Escape, KeyState::Pressed)) return false;
	
	SetComponent(nullptr);
	return true;
}

bool ComponentSelector::UpdateEditWindow() noexcept
{
	if (!m_editWindow || !m_editWindow->IsVisible()) {
		SetComponent(nullptr);
		return false;
	}

	m_editWindow->Update(IsWindowFocus(m_mainWnd));

	return m_editWindow->IsUpdateSizeOnDrag();
}

void ComponentSelector::Render()
{
	if (IsWindowFocus(m_mainWnd))
		m_tooltip->Render(m_mainWnd);

	if (m_component)
		DrawRectangle(GetRectangle(m_component), m_mainWnd);

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

bool AttachSelectedComponent(CommandList* cmdList, ComponentSelector* selector, FloatingComponent* floater,
	const XMINT2& position) noexcept
{
	UIComponent* selectComponent = selector->GetComponent();
	if (!selectComponent) return false;

	if (!AddComponentFromScreenPos(cmdList, selectComponent, floater, position))
	{
		Tool::Dialog::ShowInfoDialog(DialogType::Alert, "Attachment failed for this component.");
		return false;
	}

	return true;
}

unique_ptr<UIComponent> DetachSelectedComponent(CommandList* cmdList, ComponentSelector* selector) noexcept
{
	UIComponent* selectComponent = selector->GetComponent();
	if (!selectComponent) return nullptr;

	auto [detach, parent] = cmdList->DetachComponent(selectComponent);
	if (!detach)
	{
		Tool::Dialog::ShowInfoDialog(DialogType::Alert, "Detachment failed for this component.");
		return nullptr;
	}

	selector->SetComponent(nullptr);
	return move(detach);
}
