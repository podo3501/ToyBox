#pragma once

struct IRenderer;
class InputManager;
class UIComponent;
class ComponentTooltip;
class EditWindow;
class CommandList;

class ComponentSelector
{
public:
	~ComponentSelector();
	ComponentSelector(IRenderer* renderer, CommandList* cmdList, UIComponent* panel);

	void SetPanel(UIComponent* panel) noexcept;
	void Update(const InputManager& inputManager) noexcept;
	void Render();
	void SetMainWindow(ImGuiWindow* mainWnd) noexcept { m_mainWnd = mainWnd; }
	UIComponent* GetComponent() { return m_component; }
	void SetComponent(UIComponent* component) noexcept;

private:
	void RepeatedSelection(const vector<UIComponent*>& componentList) noexcept;
	void SelectComponent(const InputManager& inputManager) noexcept;
	bool HandleEscapeKey(const InputManager& inputManager) noexcept;
	bool UpdateEditWindow(const InputManager& inputManager) noexcept;

	IRenderer* m_renderer;
	CommandList* m_cmdList;
	ImGuiWindow* m_mainWnd;
	unique_ptr<ComponentTooltip> m_tooltip;
	unique_ptr<EditWindow> m_editWindow;

	UIComponent* m_panel;
	UIComponent* m_component;
};

/////////////////////////////////////////////////////////////////

class FloatingComponent;
bool AttachSelectedComponent(CommandList* cmdList, ComponentSelector* selector, 
	FloatingComponent* floater, const XMINT2& position) noexcept;
optional<unique_ptr<UIComponent>> DetachSelectedComponent(ComponentSelector* selector) noexcept;