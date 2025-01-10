#pragma once

struct IRenderer;
class UIComponent;
class InputManager;

class EditWindow
{
public:
	EditWindow() = delete;
	EditWindow(UIComponent* component) noexcept;
	virtual ~EditWindow();
	
	void Update(InputManager* inputManager);
	void Render(const ImGuiWindow* mainWindow);
	inline bool IsVisible() const noexcept { return m_visible; };

protected:
	virtual void UpdateComponent(InputManager* inputManager) {};
	virtual void RenderComponent(bool& modify) {};

	bool EditSize(const XMUINT2& size);

private:
	void RenderCommon(bool& modify);

	UIComponent* m_component;
	bool m_visible{ true };
};

class Panel;
class EditPanel : public EditWindow
{
public:
	~EditPanel();
	EditPanel() = delete;
	EditPanel(Panel* panel) noexcept;
	//virtual void SetComponent(UIComponent* component);
	//virtual void Render();

private:
};