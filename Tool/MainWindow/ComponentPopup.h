#pragma once

struct IRenderer;
class UIComponent;
class MouseTracker;

class ComponentPopup
{
	enum class MakeComponent
	{
		ImageGrid9
	};

public:
	ComponentPopup(IRenderer* renderer);
	~ComponentPopup();
	bool IsComponent() const noexcept;
	unique_ptr<UIComponent> GetComponent() noexcept;
	bool Excute(MouseTracker* mouseTracker);
	void Show();
	bool IsShowed() const noexcept;
	const ImVec2& GetPosition() const noexcept { return m_position; }

private:
	void Reset() noexcept;
	void DrawMakeComponent();
	bool MakeImageGrid9();

	IRenderer* m_renderer;
	unique_ptr<UIComponent> m_component;
	ImTextureID m_textureID{};
	optional<MakeComponent> m_currentAction;
	bool m_draw{ false };
	ImVec2 m_position{};
};

