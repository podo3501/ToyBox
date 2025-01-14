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
	void Update(const InputManager& inputManager) noexcept;
	void Render();
	void SetMainWindow(ImGuiWindow* mainWnd) noexcept { m_mainWnd = mainWnd; }
	UIComponent* GetComponent() { return m_component; }

private:
	void SetComponent(UIComponent* component) noexcept;
	void RepeatedSelection(const vector<UIComponent*>& componentList) noexcept;
	void SelectComponent(const InputManager& inputManager) noexcept;
	bool HandleEscapeKey(const InputManager& inputManager) noexcept;
	bool UpdateEditWindow(const InputManager& inputManager) noexcept;

	IRenderer* m_renderer;
	ImGuiWindow* m_mainWnd;
	unique_ptr<ComponentTooltip> m_tooltip;
	unique_ptr<EditWindow> m_editWindow;

	UIComponent* m_panel;
	UIComponent* m_component;
};

