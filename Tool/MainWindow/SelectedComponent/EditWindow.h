#pragma once

struct IRenderer;
class UIComponent;
class InputManager;

class EditWindow
{
public:
	EditWindow();
	virtual ~EditWindow();
	virtual void SetComponent(UIComponent* component);
	
	void Update(InputManager* inputManager);
	void Render(const ImGuiWindow* mainWindow);
	const UIComponent* GetComponent() const noexcept { return m_component; }

protected:
	virtual void UpdateComponent(InputManager* inputManager) {};
	virtual void RenderComponent(UIComponent* component, bool& modify) {};

	bool EditSize(const XMUINT2& size);

private:
	UIComponent* m_component;
	bool m_visible{ false };
};

class ComponentPanel : public EditWindow
{
public:
	//virtual void SetComponent(UIComponent* component);
	//virtual void Render();

private:
};