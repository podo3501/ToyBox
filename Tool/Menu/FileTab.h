#pragma once

class ToolSystem;
class RecentFiles;
class MainWindow;
class ResolutionSetting;
class InnerWindow;
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
	InnerWindow* GetFocusWindow() const noexcept;
	void HandleFileMenuAction(FileMenuAction action);
	bool CreateNewUIFile() noexcept;
	bool CreateMainWindow();
	bool CreateTextureResBinderWindow(const wstring& filename = L"");
	bool CreateTextureWindow();
	bool Save(InnerWindow* focusWnd, const wstring& filename = L"") const;
	bool SaveMainWindow();
	bool SaveAsMainWindow();
	bool SetResolution();

	ToolSystem* m_toolSystem;
	unique_ptr<RecentFiles> m_recentFiles;
	optional<FileMenuAction> m_currentAction; // ���� �޴� ���¸� ����
};
