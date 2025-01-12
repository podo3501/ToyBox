#pragma once

struct IRenderer;
class UIComponent;
class InputManager;
class MouseTracker;
enum class OnDrag;
namespace Tool{
	class MouseCursor;
}

class EditWindow
{
public:
	EditWindow() = delete;
	EditWindow(UIComponent* component) noexcept;
	virtual ~EditWindow();
	
	void Update(InputManager* inputManager, bool mainWndFocus);
	void Render(const ImGuiWindow* mainWindow);
	inline bool IsVisible() const noexcept { return m_visible; };
	bool IsUpdateSizeOnDrag() const noexcept;

protected:
	virtual void UpdateComponent(InputManager* inputManager) {};
	virtual void RenderComponent(bool& modify) {};

	bool EditSize(const XMUINT2& size);

private:
	void RenderCommon(bool& modify);
	void ResizeComponentOnClick(InputManager* inputManager) noexcept;
	void UpdateDragState(OnDrag dragState, const MouseTracker* mouseTracker, XMINT2& outStartPos) noexcept;
	void ResizeComponent(const XMINT2& startPos, const Mouse::State& mouseState) noexcept;

	UIComponent* m_component;
	bool m_visible{ true };
	OnDrag m_dragState;
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