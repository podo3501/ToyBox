#pragma once

struct IRenderer;
struct IInputManager;
class TextureResourceBinder;
class UIModule;
class UIComponent;
class FloatingComponent;
class ComponentSelector;
class UICommandHistory;

class ComponentController
{
public:
	~ComponentController();
	ComponentController(IRenderer* renderer, TextureResourceBinder* resBinder,
		UIComponent* mainComponent, const string& mainWndName) noexcept;

	void SetPanel(UIComponent* panel) noexcept;
	void SetUIWindow(ImGuiWindow* mainWnd) noexcept;
	bool Update() noexcept;
	void Render();
	void SetActive(bool active) noexcept;

private:
	bool ExecuteShortcutKeyCommands() noexcept;
	bool CheckAttachComponent(IInputManager* input) noexcept;
	bool CheckDetachComponent(IInputManager* input) noexcept;
	bool CheckDeleteComponent(IInputManager* input) noexcept;
	bool CheckCloneComponent(IInputManager* input) noexcept;
	bool CheckUndoComponent(IInputManager* input) noexcept;
	bool CheckRedoComponent(IInputManager* input) noexcept;

	ImGuiWindow* m_uiWindow;
	unique_ptr<UICommandHistory> m_cmdHistory;
	unique_ptr<FloatingComponent> m_floater;
	unique_ptr<ComponentSelector> m_selector;
	bool m_active{ true };
};
