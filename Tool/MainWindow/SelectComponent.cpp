#include "pch.h"
#include "SelectComponent.h"
#include "ComponentTooltip.h"
#include "ComponentWindow.h"
#include "../Toy/InputManager.h"
#include "../Toy/HelperClass.h"
#include "../Toy/Utility.h"
#include "../Toy/UserInterface/UIComponent.h"

SelectComponent::~SelectComponent() = default;
SelectComponent::SelectComponent(IRenderer* renderer, UIComponent* panel) :
	m_renderer{ renderer },
	m_tooltip{ make_unique<ComponentTooltip>(panel) },
	m_window{ nullptr },
	m_panel{ panel },
	m_component{ nullptr }
{}

void SelectComponent::SetPanel(UIComponent* panel) noexcept
{
	m_panel = panel;
	m_tooltip->SetPanel(panel);
}

static unique_ptr<ComponentWindow> CreateComponentWindow(const UIComponent* component, IRenderer* renderer)
{
	if (!component) return nullptr;
	const string& type = component->GetType();

	unique_ptr<ComponentWindow> componentWindow{ nullptr };
	if (type == "class Panel") componentWindow = make_unique<ComponentPanel>();
	if (type == "class ImageGrid1") componentWindow = make_unique<ComponentImageGrid1>();
	if (type == "class ImageGrid3") componentWindow = make_unique<ComponentImageGrid3>();
	if (type == "class ImageGrid9") componentWindow = make_unique<ComponentImageGrid9>();

	componentWindow->SetRenderer(renderer);

	return move(componentWindow);
}

void SelectComponent::SetComponent(UIComponent* component) noexcept
{
	string preType = (m_component) ? m_component->GetType() : "";
	string curType = (component) ? component->GetType() : "";

	bool change{ false };
	if (preType != curType)
		m_window = CreateComponentWindow(component, m_renderer);

	m_component = component;

	if (m_window)
		m_window->SetComponent(component);	//타입은 같지만 다른 UIComponent일지도 모른다.
}

void SelectComponent::Update(InputManager* inputManager) noexcept
{
	auto pressedKey = inputManager->GetKeyboard()->pressed;
	if (pressedKey.Escape)
	{
		SetComponent(nullptr);
		return;
	}

	auto mouseTracker = inputManager->GetMouse();
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

	if (m_window)
		m_window->Update();
}

void SelectComponent::Render(const ImGuiWindow* mainWindow)
{
	m_tooltip->Render(mainWindow);
	if (m_window)
		m_window->Render(mainWindow);
}

void SelectComponent::RepeatedSelection(const std::vector<UIComponent*>& componentList) noexcept
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

