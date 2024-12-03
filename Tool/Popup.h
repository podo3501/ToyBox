#pragma once

struct IRenderer;
class UIComponent;
class MouseTracker;

namespace Tool
{
	class Popup
	{
		enum class MakeComponent
		{
			Dialog
		};

	public:
		Popup(IRenderer* renderer);
		~Popup();
		bool IsComponent() const noexcept;
		unique_ptr<UIComponent> GetComponent() noexcept;
		bool Excute(MouseTracker* mouseTracker);
		void Show();
		const ImVec2& GetPosition() const noexcept { return m_position; }

	private:
		void Reset() noexcept;
		void DrawMakeComponent();
		bool MakeDialog();

		IRenderer* m_renderer;
		unique_ptr<UIComponent> m_component;
		ImTextureID m_textureID{};
		optional<MakeComponent> m_currentAction;
		bool m_draw{ false };
		ImVec2 m_position{};
	};
}
