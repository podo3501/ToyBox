#pragma once

struct IRenderer;
class UIComponent;
class InputManager;
class FloatingComponent;
class ComponentSelector;

class ComponentController
{
public:
	~ComponentController();
	ComponentController(IRenderer* renderer,
		UIComponent* panel, const string& mainWndNam) noexcept;

	void SetPanel(UIComponent* panel) noexcept;
	void SetMainWindow(ImGuiWindow* mainWnd) noexcept;
	bool Update(const InputManager& inputManager) noexcept;
	void Render();

private:
	bool CheckAttachComponent(const InputManager& inputManager) noexcept;
	bool CheckDetachComponent(const InputManager& inputManager) noexcept;
	bool CheckDeleteComponent(const InputManager& inputManager) noexcept;

	unique_ptr<FloatingComponent> m_floater;
	unique_ptr<ComponentSelector> m_selector;
};
