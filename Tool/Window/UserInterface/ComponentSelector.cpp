#include "pch.h"
#include "ComponentSelector.h"
#include "SelectedComponent/ComponentEdit/EditWindow.h"
#include "SelectedComponent/ComponentTooltip.h"
#include "SelectedComponent/EditWindowFactory.h"
#include "Toy/UserInterface/UIComponent/UIComponent.h"
#include "Toy/UserInterface/CommandHistory/UserInterface/UICommandHistory.h"
#include "Shared/System/Public/IInputManager.h"
#include "Shared/Framework/Locator.h"
#include "Shared/Utils/StlExt.h"
#include "../Dialog.h"
#include "Window/Utils/Common.h"

ComponentSelector::~ComponentSelector() = default;
ComponentSelector::ComponentSelector(IRenderer* renderer, TextureResourceBinder* resBinder, 
	UIComponent* mainComponent, UICommandHistory* cmdHistory) :
	m_renderer{ renderer },
	m_resBinder{ resBinder },
	m_cmdHistory{ cmdHistory },
	m_uiWindow{ nullptr },
	m_tooltip{ make_unique<ComponentTooltip>(mainComponent) },
	m_editWindow{ nullptr },
	m_mainComponent{ mainComponent },
	m_component{ nullptr }
{
	EditWindowFactory::RegisterFactories();
}


void ComponentSelector::SetPanel(UIComponent* panel) noexcept
{
	m_mainComponent = panel;
	m_tooltip->SetPanel(panel);
}

void ComponentSelector::SetComponent(UIComponent* component) noexcept
{
	if (m_component == nullptr && component == nullptr) return;
	if (m_component == component)
	{
		SetComponent(nullptr);
		return;
	}

	m_editWindow = EditWindowFactory::CreateEditWindow(component, m_cmdHistory, m_resBinder);
	if (m_editWindow) m_editWindow->Setup();

	m_component = component;
	m_tooltip->SetComponent(component);
}

void ComponentSelector::SelectComponent(IInputManager* input) noexcept
{
	if (!m_uiWindow) return;
	if (!input->IsInputAction(MouseButton::Left, InputState::Pressed)) return;

	static vector<UIComponent*> preComponentList{ nullptr };
	const XMINT2& pos = input->GetPosition();
	vector<UIComponent*> componentList = UIEx(m_mainComponent).FindRenderComponents(pos);
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
	auto input = Locator<IInputManager>::GetService();
	if (HandleEscapeKey(input)) return;
	if (UpdateEditWindow()) return;

	if (IsWindowFocus(m_uiWindow))
		SelectComponent(input);
}

bool ComponentSelector::HandleEscapeKey(IInputManager* input) noexcept
{
	if (!input->IsInputAction(Keyboard::Escape, InputState::Pressed)) return false;
	
	SetComponent(nullptr);
	return true;
}

bool ComponentSelector::UpdateEditWindow() noexcept
{
	if (!m_editWindow || !m_editWindow->IsVisible()) {
		SetComponent(nullptr);
		return false;
	}

	m_editWindow->Update(IsWindowFocus(m_uiWindow));

	return m_editWindow->IsUpdateSizeOnDrag();
}

void ComponentSelector::Render()
{
	if (IsWindowFocus(m_uiWindow))
		m_tooltip->Render(m_uiWindow);

	if (m_component)
		DrawRectangle(m_uiWindow, m_component->GetArea(), ToColor(Colors::White), ToColor(Colors::White, 0.3f));

	if (m_editWindow)
		m_editWindow->Render(m_uiWindow);
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

bool AttachSelectedComponent(UICommandHistory* cmdHistory, 
	ComponentSelector* selector, FloatingComponent* floater, const XMINT2& position) noexcept
{
	UIComponent* selectComponent = selector->GetComponent();
	if (!selectComponent) return false;

	if (!AddComponentFromScreenPos(cmdHistory, selectComponent, floater, position))
	{
		Tool::Dialog::ShowInfoDialog(DialogType::Alert, "Attachment failed for this component.");
		return false;
	}

	return true;
}

unique_ptr<UIComponent> DetachSelectedComponent(UICommandHistory* cmdHistory, ComponentSelector* selector) noexcept
{
	UIComponent* selectComponent = selector->GetComponent();
	if (!selectComponent) return nullptr;

	auto [detach, parent] = cmdHistory->DetachComponent(selectComponent);
	if (!detach)
	{
		Tool::Dialog::ShowInfoDialog(DialogType::Alert, "Detachment failed for this component.");
		return nullptr;
	}

	selector->SetComponent(nullptr);
	return move(detach);
}
