#include "pch.h"
#include "ComponentSelector.h"
#include "ComponentTooltip.h"
#include "ComponentWindow.h"
#include "../Toy/InputManager.h"
#include "../Toy/HelperClass.h"
#include "../Toy/Utility.h"
#include "../Toy/UserInterface/UIComponent.h"
#include "../../Utility.h"

ComponentSelector::~ComponentSelector() = default;
ComponentSelector::ComponentSelector(IRenderer* renderer, UIComponent* panel) :
	m_renderer{ renderer },
	m_tooltip{ make_unique<ComponentTooltip>(panel) },
	m_window{ nullptr },
	m_panel{ panel },
	m_component{ nullptr }
{}

void ComponentSelector::SetPanel(UIComponent* panel) noexcept
{
	m_panel = panel;
	m_tooltip->SetPanel(panel);
}

static unique_ptr<ComponentWindow> CreateComponentWindow(const UIComponent* component, IRenderer* renderer)
{
	if (!component) return nullptr;

	const string& type = component->GetType();
	static const unordered_map<string, function<unique_ptr<ComponentWindow>()>> factoryMap{
		{ "class Panel", []() { return make_unique<ComponentPanel>(); } },
		{ "class ImageGrid1", [renderer]() { return make_unique<ComponentImageGrid1>(renderer); } },
		{ "class ImageGrid3", []() { return make_unique<ComponentImageGrid3>(); } },
		{ "class ImageGrid9", []() { return make_unique<ComponentImageGrid9>(); } },
	};

	auto it = factoryMap.find(type);
	if (it == factoryMap.end()) return nullptr;

	return it->second();
}

void ComponentSelector::SetComponent(UIComponent* component) noexcept
{
	string preType = (m_component) ? m_component->GetType() : "";
	string curType = (component) ? component->GetType() : "";

	bool change{ false };
	if (preType != curType)
		m_window = CreateComponentWindow(component, m_renderer);

	if (m_window)
		m_window->SetComponent(component);	//Ÿ���� ������ �ٸ� UIComponent������ �𸥴�.

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
	else //���콺�� �ٸ� ������Ʈ�� �����ؼ� ����Ʈ�� �ٲ���ٸ� ���� ù��° ���Ҹ� ����ش�.
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

	if (m_window)
		m_window->Update();

	if (!bPopupActive)	//�˾��� Ȱ�����̸�
		SelectComponent(inputManager);
}

void ComponentSelector::Render(const ImGuiWindow* mainWindow, bool bPopupActive)
{
	if(!bPopupActive && IsWindowFocus(mainWindow)) //component�� ���� �������Ѽ� ���Ϸ��� �Ѵٸ� ������ ������ �ʰ� �Ѵ�.
		m_tooltip->Render(mainWindow);

	if(m_component)
		DrawRectangle(m_component->GetRectangle(), mainWindow);

	if (m_window)
		m_window->Render(mainWindow);
}

void ComponentSelector::RepeatedSelection(const std::vector<UIComponent*>& componentList) noexcept
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
