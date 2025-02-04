#pragma once

struct IRenderer;
class CommandList;
class UIComponent;
class MouseTracker;
enum class OnDrag;
namespace Tool{
	class MouseCursor;
}

class EditWindow
{
public:
	EditWindow() = delete;
	EditWindow(UIComponent* component, CommandList* cmdList) noexcept;
	virtual ~EditWindow();
	
	void Update(bool mainWndFocus);
	void Render(const ImGuiWindow* mainWindow);
	inline bool IsVisible() const noexcept { return m_visible; };
	bool IsUpdateSizeOnDrag() const noexcept;

protected:
	virtual void UpdateComponent() {};
	virtual void RenderComponent() {};

	bool EditSize(const XMUINT2& size);
	inline CommandList* GetCommandList() const noexcept { return m_cmdList; }

private:
	void RenderCommon();
	void ResizeComponentOnClick() noexcept;
	void UpdateDragState(OnDrag dragState, XMINT2& outStartPos) noexcept;
	void ResizeComponent(const XMINT2& startPos, const Mouse::State& mouseState) noexcept;
	void EditName(const string& nameLabel) noexcept;

	UIComponent* m_component;
	CommandList* m_cmdList;
	char m_nameBuffer[128] = "";
	bool m_visible{ true };
	OnDrag m_dragState;
};

class Panel;
class EditPanel : public EditWindow
{
public:
	~EditPanel();
	EditPanel() = delete;
	EditPanel(Panel* panel, CommandList* cmdList) noexcept;
	//virtual void SetComponent(UIComponent* component);
	//virtual void Render();

private:
};