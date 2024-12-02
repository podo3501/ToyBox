#pragma once

struct IRenderer;
class Scene;
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
		bool Excute(MouseTracker* mouseTracker);
		void Show();

	private:
		void DrawMakeComponent();
		bool CreateScene(const XMUINT2& size);
		bool MakeDialog();

		IRenderer* m_renderer;
		unique_ptr<Scene> m_scene;
		ImTextureID m_textureID{};
		optional<MakeComponent> m_currentAction;
		bool m_draw{ false };
		ImVec2 m_position{};
	};
}
