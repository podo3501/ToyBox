#pragma once

struct IRenderer;
class InputManager;
class UIComponent;
class ComponentTooltip;
class ComponentWindow;

class SelectComponent
{
public:
	~SelectComponent();
	SelectComponent(IRenderer* renderer, UIComponent* panel);

	void SetPanel(UIComponent* panel) noexcept;
	void SetComponent(UIComponent* component) noexcept;
	void Update(InputManager* inputManager) noexcept;
	void Render(const ImGuiWindow* mainWindow);

private:
	void RepeatedSelection(const std::vector<UIComponent*>& componentList) noexcept;

	IRenderer* m_renderer;
	unique_ptr<ComponentTooltip> m_tooltip;
	unique_ptr<ComponentWindow> m_window;

	UIComponent* m_panel;
	UIComponent* m_component;
};

