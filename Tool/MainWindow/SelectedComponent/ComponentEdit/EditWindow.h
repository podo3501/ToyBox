#pragma once

struct IRenderer;
class UICommandList;
class UIComponent;
class TextureSourceBinder;
class RenameNotifier;
class MouseTracker;
enum class OnDrag;
namespace Tool{
	class MouseCursor;
}

class EditWindow
{
public:
	EditWindow() = delete;
	EditWindow(UIComponent* component, TextureSourceBinder* sourceBinder, UICommandList* cmdList) noexcept;
	virtual ~EditWindow();
	
	void Update(bool mainWndFocus);
	void Render(const ImGuiWindow* mainWindow);
	inline bool IsVisible() const noexcept { return m_visible; };
	bool IsUpdateSizeOnDrag() const noexcept;

protected:
	virtual void UpdateComponent() {};
	virtual void RenderComponent() {};

	inline UICommandList* GetUICommandList() const noexcept { return m_cmdList; }
	inline UIComponent* GetComponent() const noexcept { return m_component; }
	inline TextureSourceBinder* GetTextureSourceBinder() const noexcept { return m_sourceBinder; }

private:
	void RenderCommon();
	void ResizeComponentOnClick() noexcept;
	void UpdateDragState(OnDrag dragState, XMINT2& outStartPos) noexcept;
	void ResizeComponent(const XMINT2& startPos, const Mouse::State& mouseState) noexcept;

	UIComponent* m_component;
	TextureSourceBinder* m_sourceBinder;
	UICommandList* m_cmdList;
	unique_ptr<RenameNotifier> m_renameNotifier;
	bool m_visible{ true };
	OnDrag m_dragState;
};

class Panel;
class EditPanel : public EditWindow
{
public:
	~EditPanel();
	EditPanel() = delete;
	EditPanel(Panel* panel, TextureSourceBinder* sourceBinder, UICommandList* cmdList) noexcept;
	//virtual void SetComponent(UIComponent* component);
	//virtual void Render();

private:
};