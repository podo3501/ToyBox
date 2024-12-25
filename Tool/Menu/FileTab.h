#pragma once

class ToolSystem;
class RecentFiles;
class MainWindow;
class ResolutionSetting;

class FileTab
{
	enum class FileMenuAction
	{
		None,
		NewFile,
		OpenFile,
		OpenRecent,
		SaveFile,
		SaveAsFile,
		Resolution,
		Quit
	};

public:
	FileTab(ToolSystem* toolSystem);
	~FileTab();

	void Show();
	bool Excute();
	bool CreateMainWindowFromFile(const wstring& filename);

private:
	MainWindow* GetFocusWindow() const noexcept;
	void HandleFileMenuAction(FileMenuAction action);
	bool NewFile() noexcept;
	bool CreateMainWindow();
	bool Save(MainWindow* focusWnd, const wstring& filename = L"") const;
	bool SaveMainWindow();
	bool SaveAsMainWindow();
	bool SetResolution();

private:
	ToolSystem* m_toolSystem;
	unique_ptr<RecentFiles> m_recentFiles;
	optional<FileMenuAction> m_currentAction; // 현재 메뉴 상태를 저장
};
