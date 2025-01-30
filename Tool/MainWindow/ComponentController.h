#pragma once

struct IRenderer;
class UIComponent;
class FloatingComponent;
class ComponentSelector;
class CommandList;

class ComponentController
{
public:
	~ComponentController();
	ComponentController(IRenderer* renderer,
		UIComponent* panel, const string& mainWndNam) noexcept;

	void SetPanel(UIComponent* panel) noexcept;
	void SetMainWindow(ImGuiWindow* mainWnd) noexcept;
	bool Update() noexcept;
	void Render();

private:
	bool CheckAttachComponent() noexcept;
	bool CheckDetachComponent() noexcept;
	bool CheckDeleteComponent() noexcept;
	bool CheckCloneComponent() noexcept;
	bool CheckUndoComponent() noexcept;
	bool CheckRedoComponent() noexcept;

	ImGuiWindow* m_mainWnd;
	unique_ptr<CommandList> m_cmdList;
	unique_ptr<FloatingComponent> m_floater;
	unique_ptr<ComponentSelector> m_selector;
};
