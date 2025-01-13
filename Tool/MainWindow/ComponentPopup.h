#pragma once

struct IRenderer;
class UIComponent;
class MouseTracker;

class ComponentPopup
{
	enum class MakeComponent
	{
		ImageGrid1,
		ImageGrid3,
		ImageGrid9
	};

public:
	ComponentPopup(IRenderer* renderer);
	~ComponentPopup();
	bool IsComponent() const noexcept;
	unique_ptr<UIComponent> GetComponent() noexcept;
	bool Excute();
	void Render();
	bool IsActive() const noexcept;
	const ImVec2& GetPosition() const noexcept { return m_position; }

private:
	void Reset() noexcept;
	void DrawMakeComponent();
	bool LoadImageGrid(unique_ptr<UIComponent>&& imgGrid);
	bool MakeImageGrid1();
	bool MakeImageGrid3();
	bool MakeImageGrid9();

	IRenderer* m_renderer;
	unique_ptr<UIComponent> m_component;
	ImTextureID m_textureID{};
	optional<MakeComponent> m_currentAction;
	bool m_draw{ false };
	bool m_isActive{ false };
	ImVec2 m_position{};
};

