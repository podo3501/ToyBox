#pragma once

struct IRenderer;
class TextureSourceBinder;
class RenderTexture;
class UIComponent;
class MouseTracker;

//팝업 메뉴와 floatingComponent 두개의 역할이 있다 나눌 필요가 있을듯
class FloatingComponent
{
	enum class MakeComponent
	{
		ImageGrid1,
		ImageGrid3,
		ImageGrid9,
		ImageSwitcher1,
		ImageSwitcher3,
		Button1,
		Button3,
		Text,
		ListArea
	};

public:
	FloatingComponent(IRenderer* renderer, TextureSourceBinder* sourceBinder, const string& mainWndName) noexcept;
	~FloatingComponent();
	bool IsComponent() const noexcept;
	unique_ptr<UIComponent> GetComponent() noexcept;
	void SetComponent(unique_ptr<UIComponent> componen) noexcept;
	bool Excute();
	void Render();
	bool IsActive() const noexcept { return m_isActive; }
	const ImVec2& GetPosition() const noexcept { return m_position; }
	bool ComponentToFloating(unique_ptr<UIComponent>&& component);

private:
	void Clear() noexcept;
	bool LoadComponentInternal(unique_ptr<UIComponent>&& component, const XMUINT2& size);
	bool LoadComponent(unique_ptr<UIComponent> component);
	void DrawMakeComponent();

	IRenderer* m_renderer;
	TextureSourceBinder* m_sourceBinder;
	unique_ptr<RenderTexture> m_renderTex;
	string m_name;
	UIComponent* m_component;
	optional<MakeComponent> m_currentAction;
	bool m_isActive{ false };
	ImVec2 m_position{};
};

