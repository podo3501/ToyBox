#pragma once

class FileTab;
class JsonOperation;

class RecentFiles
{
    static constexpr size_t MaxRecentFiles = 20;    //More �� ��ģ �� ����
    static constexpr size_t MaxShownFiles = 7;    // "Open Recent" �޴��� ǥ���� ���� ����
    static constexpr const wchar_t* RecentFilename = L"Tool/OpenRecentFiles.json";

public:
    RecentFiles();

    void AddFile(const wstring& filename);
    bool OpenFile(FileTab& menuBar);
    bool Show();
    void SerializeIO(JsonOperation& jsonOp);

private:
    void ShowMoreMenu();

    deque<wstring> m_recentFiles{};
    wstring m_file{};
};
