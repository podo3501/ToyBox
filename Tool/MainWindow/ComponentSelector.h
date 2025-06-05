#pragma once

struct IRenderer;
class UIModule;
class UIComponent;
class ComponentTooltip;
class TextureResourceBinder;
class EditWindow;
class UICommandList;

class ComponentSelector
{
public:
	~ComponentSelector();
	ComponentSelector(IRenderer* renderer, UIModule* uiModule, UICommandList* cmdList);

	void SetPanel(UIComponent* panel) noexcept;
	void Update() noexcept;
	void Render();
	void SetMainWindow(ImGuiWindow* mainWnd) noexcept { m_mainWnd = mainWnd; }
	UIComponent* GetComponent() { return m_component; }
	void SetComponent(UIComponent* component) noexcept;

private:
	void RepeatedSelection(const vector<UIComponent*>& componentList) noexcept;
	void SelectComponent() noexcept;
	bool HandleEscapeKey() noexcept;
	bool UpdateEditWindow() noexcept;

	IRenderer* m_renderer;
	UIModule* m_uiModule;
	UICommandList* m_cmdList;
	ImGuiWindow* m_mainWnd;
	unique_ptr<ComponentTooltip> m_tooltip;
	unique_ptr<EditWindow> m_editWindow;

	UIComponent* m_mainComponent;
	UIComponent* m_component;
};

/////////////////////////////////////////////////////////////////

class FloatingComponent;
bool AttachSelectedComponent(UICommandList* cmdList, UIModule* uiModule,
	ComponentSelector* selector, FloatingComponent* floater, const XMINT2& position) noexcept;
unique_ptr<UIComponent> DetachSelectedComponent(UICommandList* cmdList,
	UIModule* uiModule, ComponentSelector* selector) noexcept;