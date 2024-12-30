#pragma once

struct IRenderer;
class InputManager;
class UIComponent;
class ComponentTooltip;
class ComponentWindow;

class ComponentSelector
{
public:
	~ComponentSelector();
	ComponentSelector(IRenderer* renderer, UIComponent* panel);

	void SetPanel(UIComponent* panel) noexcept;
	void SetComponent(UIComponent* component) noexcept;
	void Update(InputManager* inputManager, bool bPopupActive) noexcept;
	void Render(const ImGuiWindow* mainWindow, bool bPopupActive);

private:
	void RepeatedSelection(const std::vector<UIComponent*>& componentList) noexcept;
	void SelectComponent(InputManager* inputManager) noexcept;

	IRenderer* m_renderer;
	unique_ptr<ComponentTooltip> m_tooltip;
	unique_ptr<ComponentWindow> m_window;

	UIComponent* m_panel;
	UIComponent* m_component;
};

