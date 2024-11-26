#pragma once

struct IRenderer;
class ToolSystem;
class MainWindow;

namespace Tool
{
	class Popup;
}

namespace Tool
{
	class MainMenuBar
	{
		enum class FileMenuAction
		{
			None,
			NewFile,
			OpenFile,
			Quit
		};

	public:
		MainMenuBar() = delete;
		MainMenuBar(ToolSystem* toolSystem, Popup* popup);
		~MainMenuBar();

		void Update();
		bool Excute(); //Render���� ȣ���ؾ���.

	private:
		void ShowMainMenuBar();
		void RenderFileMenu();
		bool HandleFileMenu();
		void HandleFileMenuAction(FileMenuAction action);

		bool CreateMainWindowFromFile();

		ToolSystem* m_toolSystem;
		Tool::Popup* m_popup;
		std::optional<FileMenuAction> m_currentAction; // ���� �޴� ���¸� ����
	};
}