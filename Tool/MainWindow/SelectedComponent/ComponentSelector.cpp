#include "pch.h"
#include "ComponentSelector.h"
#include "ComponentTooltip.h"
#include "ComponentEdit/EditImageGrid.h"
#include "../Toy/InputManager.h"
#include "../Toy/HelperClass.h"
#include "../Toy/Utility.h"
#include "../Toy/UserInterface/UIComponent.h"
#include "../../Utility.h"

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

static unique_ptr<EditWindow> CreateEditWindow(const UIComponent* component, IRenderer* renderer)
{
	if (!component) return nullptr;

	const string& type = component->GetType();
	static const unordered_map<string, function<unique_ptr<EditWindow>()>> factoryMap{
		{ "class Panel", []() { return make_unique<ComponentPanel>(); } },
		{ "class ImageGrid1", [renderer]() { return make_unique<EditImageGrid1>(renderer); } },
		{ "class ImageGrid3", [renderer]() { return make_unique<EditImageGrid3>(renderer); } },
		{ "class ImageGrid9", []() { return make_unique<EditImageGrid9>(); } },
	};

	auto it = factoryMap.find(type);
	if (it == factoryMap.end()) return nullptr;

	return it->second();
}

void ComponentSelector::SetComponent(UIComponent* component) noexcept
{
	if (m_component == nullptr && component == nullptr) return;
	if (m_component == component) 
		return;

	m_editWindow = CreateEditWindow(component, m_renderer);
	if(m_editWindow)
		m_editWindow->SetComponent(component);

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

