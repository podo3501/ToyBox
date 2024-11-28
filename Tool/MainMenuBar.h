#pragma once

struct IRenderer;
class ToolSystem;
class MainWindow;
class RecentFiles;

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
			OpenRecent,
			Quit
		};

	public:
		MainMenuBar() = delete;
		MainMenuBar(ToolSystem* toolSystem, Popup* popup);
		~MainMenuBar();

		void Update();
		bool Excute(); //Render에서 호출해야함.

	private:
		void ShowMainMenuBar();
		void RenderFileMenu();
		bool HandleFileMenu();
		void HandleFileMenuAction(FileMenuAction action);

		bool NewFile() noexcept;
		bool CreateMainWindowFromFile();
		bool OpenRecentFile();

		ToolSystem* m_toolSystem;
		Tool::Popup* m_popup;
		unique_ptr<RecentFiles> m_recentFiles;
		std::optional<FileMenuAction> m_currentAction; // 현재 메뉴 상태를 저장
	};
}