#pragma once

class ToolSystem;
class RecentFiles;
class MainWindow;
class ResolutionSetting;
enum class FileMenuAction;

class FileTab
{
public:
	FileTab(ToolSystem* toolSystem);
	~FileTab();

	void Show();
	bool Excute();
	bool CreateMainWindowFromFile(const wstring& filename);

private:
	MainWindow* GetFocusWindow() const noexcept;
	void HandleFileMenuAction(FileMenuAction action);
	bool CreateNewUIFile() noexcept;
	bool CreateNewTextureFile() noexcept;
	bool CreateMainWindow();
	bool CreateTextureWindowFromFile(const wstring& filename);
	bool CreateTextureWindow();
	bool Save(MainWindow* focusWnd, const wstring& filename = L"") const;
	bool SaveMainWindow();
	bool SaveAsMainWindow();
	bool SetResolution();

private:
	ToolSystem* m_toolSystem;
	unique_ptr<RecentFiles> m_recentFiles;
	optional<FileMenuAction> m_currentAction; // 현재 메뉴 상태를 저장
};
