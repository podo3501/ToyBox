#pragma once

struct IRenderer;
class InputManager;
class UIComponent;
class ComponentTooltip;
class EditWindow;

class ComponentSelector
{
public:
	~ComponentSelector();
	ComponentSelector(IRenderer* renderer, UIComponent* panel);

	void SetPanel(UIComponent* panel) noexcept;
	void SetComponent(UIComponent* component) noexcept;
	void Update(InputManager* inputManager, bool bPopupActive) noexcept;
	void Render(bool bPopupActive);
	void SetMainWindow(ImGuiWindow* mainWnd) noexcept { m_mainWnd = mainWnd; }

private:
	void RepeatedSelection(const vector<UIComponent*>& componentList) noexcept;
	void SelectComponent(InputManager* inputManager) noexcept;

	IRenderer* m_renderer;
	ImGuiWindow* m_mainWnd;
	unique_ptr<ComponentTooltip> m_tooltip;
	unique_ptr<EditWindow> m_editWindow;

	UIComponent* m_panel;
	UIComponent* m_component;
};

