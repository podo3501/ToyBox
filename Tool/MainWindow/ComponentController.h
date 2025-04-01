#pragma once

struct IRenderer;
class TextureSourceBinder;
class UIComponent;
class FloatingComponent;
class ComponentSelector;
class UICommandList;

class ComponentController
{
public:
	~ComponentController();
	ComponentController(IRenderer* renderer, TextureSourceBinder* sourceBinder,
		UIComponent* panel, const string& mainWndNam) noexcept;

	void SetPanel(UIComponent* panel) noexcept;
	void SetMainWindow(ImGuiWindow* mainWnd) noexcept;
	bool Update() noexcept;
	void Render();
	void SetActive(bool active) noexcept;

private:
	bool ExecuteShortcutKeyCommands() noexcept;
	bool CheckAttachComponent() noexcept;
	bool CheckDetachComponent() noexcept;
	bool CheckDeleteComponent() noexcept;
	bool CheckCloneComponent() noexcept;
	bool CheckUndoComponent() noexcept;
	bool CheckRedoComponent() noexcept;

	ImGuiWindow* m_mainWnd;
	unique_ptr<UICommandList> m_cmdList;
	unique_ptr<FloatingComponent> m_floater;
	unique_ptr<ComponentSelector> m_selector;
	bool m_active{ true };
};
