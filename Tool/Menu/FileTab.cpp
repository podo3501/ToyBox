#include "pch.h"
#include "FileTab.h"
#include "../Dialog.h"
#include "../Toy/Window.h"
#include "../Toy/Utility.h"
#include "../Toy/Config.h"
#include "../ToolSystem.h"
#include "../MainWindow/MainWindow.h"
#include "MenuHelper.h"
#include "RecentFiles.h"
#include "../Config.h"

using namespace Tool;

FileTab::~FileTab() = default;
FileTab::FileTab(ToolSystem* toolSystem) :
    m_toolSystem{ toolSystem },
    m_recentFiles{ make_unique<RecentFiles>() }
{}

//Render에서 실행
void FileTab::Show()
{
    if (ImGui::MenuItem("New")) HandleFileMenuAction(FileMenuAction::NewFile);
    if (ImGui::BeginMenu("Open"))
    {
        if (ImGui::MenuItem("UI File", "Ctrl+O")) HandleFileMenuAction(FileMenuAction::OpenUIFile);
        if (ImGui::MenuItem("Texture File")) HandleFileMenuAction(FileMenuAction::OpenTextureFile);
        ImGui::EndMenu();
    }
    if (m_recentFiles->Show()) HandleFileMenuAction(FileMenuAction::OpenRecent);
    if (ImGui::MenuItem("Save", "Ctrl+S")) HandleFileMenuAction(FileMenuAction::SaveFile);
    if (ImGui::MenuItem("Save As..")) HandleFileMenuAction(FileMenuAction::SaveAsFile);

    ImGui::Separator();

    if(ResolutionSettingShow()) HandleFileMenuAction(FileMenuAction::Resolution);

    ImGui::Separator();

    if (ImGui::MenuItem("Quit")) HandleFileMenuAction(FileMenuAction::Quit);
}

void FileTab::HandleFileMenuAction(FileMenuAction action)
{
    m_currentAction = action; // 메뉴는 한번에 하나씩 클릭하기 때문에 상태가 하나만 저장된다.
}

//update에서 실행해야 함.
bool FileTab::Excute()
{
    if (!m_currentAction.has_value()) return true;

    auto result{ true };
    switch (m_currentAction.value())
    {
    case FileMenuAction::NewFile: result = NewFile();  break;
    case FileMenuAction::OpenUIFile: result = CreateMainWindow(); break;
    //case FileMenuAction::OpenTextureFile: result = Create
    case FileMenuAction::OpenRecent: result = m_recentFiles->OpenFile(*this); break;
    case FileMenuAction::SaveFile: result = SaveMainWindow(); break;
    case FileMenuAction::SaveAsFile: result = SaveAsMainWindow(); break;
    case FileMenuAction::Resolution: result = SetResolution(); break;

    case FileMenuAction::Quit: Window::ExitGame(); break;
    default:
        break;
    }
    m_currentAction.reset(); // 상태 초기화

    return result;
}

bool FileTab::NewFile() noexcept
{
    auto mainWindow = std::make_unique<MainWindow>(m_toolSystem->GetRenderer());
    const XMUINT2& resolution = Config::GetResolutionInCoordinate();
    ReturnIfFalse(mainWindow->CreateScene(resolution));

    m_toolSystem->SetMainWindow(move(mainWindow));

    return true;
}

bool FileTab::CreateMainWindowFromFile(const wstring& filename)
{
    auto mainWindow = std::make_unique<MainWindow>(m_toolSystem->GetRenderer());
    if (!mainWindow->CreateScene(filename))
    {
        Tool::Dialog::ShowInfoDialog(DialogType::Error, "Failed to open the file. Please check the file path.");
        return false;
    }

    m_toolSystem->SetMainWindow(move(mainWindow));

    return true;
}

bool FileTab::CreateMainWindow()
{
    wstring relativePath{};
    GetRelativePathFromDialog(relativePath);
    if (relativePath.empty()) return true;

    auto create = CreateMainWindowFromFile(relativePath);
    if (create)
        m_recentFiles->AddFile(relativePath);

    return true;
}

MainWindow* FileTab::GetFocusWindow() const noexcept
{
    return m_toolSystem->GetFocusMainWindow();
}

bool FileTab::Save(MainWindow* focusWnd, const wstring& filename) const 
{
    auto result = focusWnd->SaveScene(filename);
    if (result)
    {
        const wstring& curFilename = focusWnd->GetSaveFilename();
        m_recentFiles->AddFile(curFilename);
        Tool::Dialog::ShowInfoDialog(DialogType::Message, "Saved to " + WStringToString(curFilename));
    }
    else
        Tool::Dialog::ShowInfoDialog(DialogType::Error, "Failed to save the file.");

    return result;
}

bool FileTab::SaveMainWindow()
{
    MainWindow* focusWnd = GetFocusWindow();
    if (focusWnd == nullptr)
    {
        Tool::Dialog::ShowInfoDialog(DialogType::Alert, "There's no Main Window available to save.");
        return true;
    }

    return Save(focusWnd);
}

bool FileTab::SaveAsMainWindow()
{
    MainWindow* focusWnd = GetFocusWindow();
    if (focusWnd == nullptr)
    {
        Tool::Dialog::ShowInfoDialog(DialogType::Alert, "There's no Main Window available to save.");
        return true;
    }

    wstring selectedFilename{};
    GetRelativePathFromDialog(selectedFilename);
    if (selectedFilename.empty())
        return true;

    return Save(focusWnd, selectedFilename);
}

bool FileTab::SetResolution()
{
    //MainWindow* focusWnd = GetFocusWindow();
    //if (focusWnd == nullptr)
    //    return true;

    //XMUINT2 resolution = Config::GetResolutionInCoordinate();
    //focusWnd->ChangeWindowSize({ static_cast<float>(resolution.x), static_cast<float>(resolution.y) });

    return true;
}
