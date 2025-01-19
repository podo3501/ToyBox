#pragma once

struct IRenderer;
class UIComponent;
class MouseTracker;

//팝업 메뉴와 floatingComponent 두개의 역할이 있다 나눌 필요가 있을듯
class FloatingComponent
{
	enum class MakeComponent
	{
		ImageGrid1,
		ImageGrid3,
		ImageGrid9
	};

public:
	FloatingComponent(IRenderer* renderer, const string& mainWndName) noexcept;
	~FloatingComponent();
	bool IsComponent() const noexcept;
	unique_ptr<UIComponent> GetComponent() noexcept;
	void SetComponent(unique_ptr<UIComponent> componen) noexcept;
	bool Excute();
	void Render();
	bool IsActive() const noexcept;
	const ImVec2& GetPosition() const noexcept { return m_position; }
	bool ComponentToFloating(unique_ptr<UIComponent>&& component);
	void Clear() noexcept;

private:
	bool LoadComponentInternal(unique_ptr<UIComponent>&& component, const XMUINT2& size);
	bool LoadComponent(unique_ptr<UIComponent>&& component);
	void DrawMakeComponent();

	IRenderer* m_renderer;
	string m_name;
	unique_ptr<UIComponent> m_component;
	ImTextureID m_textureID{};
	optional<MakeComponent> m_currentAction;
	XMUINT2 m_drawTextureSize{};
	bool m_draw{ false };
	bool m_isActive{ false };
	ImVec2 m_position{};
};

