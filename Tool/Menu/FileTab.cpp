#include "pch.h"
#include "FileTab.h"
#include "../Popup.h"
#include "../Toy/Window.h"
#include "../Toy/Utility.h"
#include "../ToolSystem.h"
#include "../MainWindow.h"
#include "MenuHelper.h"
#include "RecentFiles.h"
#include "../Config.h"

using namespace Tool;

FileTab::~FileTab() = default;
FileTab::FileTab(ToolSystem* toolSystem, Popup* popup) :
    m_toolSystem{ toolSystem },
    m_popup{ popup },
    m_recentFiles{ make_unique<RecentFiles>() }
{}

//Render에서 실행
void FileTab::Show()
{
    if (ImGui::MenuItem("New")) HandleFileMenuAction(FileMenuAction::NewFile);
    if (ImGui::MenuItem("Open", "Ctrl+O")) HandleFileMenuAction(FileMenuAction::OpenFile);
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
    case FileMenuAction::OpenFile: result = CreateMainWindow(); break;
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
        m_popup->ShowInfoDialog(DialogType::Error, "Failed to open the file. Please check the file path.");
        return false;
    }

    m_toolSystem->SetMainWindow(move(mainWindow));

    return true;
}

bool FileTab::CreateMainWindow()
{
    wstring selectedFilename{};
    if (!m_popup->ShowFileDialog(selectedFilename, FileDialogType::Open))
        return false;

    if (selectedFilename.empty())   //다이얼로그를 닫거나 취소를 눌리면 파일명이 없다.
        return true;

    auto create = CreateMainWindowFromFile(selectedFilename);
    if (create)
        m_recentFiles->AddFile(selectedFilename);

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
        m_popup->ShowInfoDialog(DialogType::Message, "Saved to " + WStringToString(curFilename));
    }
    else
        m_popup->ShowInfoDialog(DialogType::Error, "Failed to save the file.");

    return result;
}

bool FileTab::SaveMainWindow()
{
    MainWindow* focusWnd = GetFocusWindow();
    if (focusWnd == nullptr)
    {
        m_popup->ShowInfoDialog(DialogType::Alert, "There's no Main Window available to save.");
        return true;
    }

    return Save(focusWnd);
}

bool FileTab::SaveAsMainWindow()
{
    MainWindow* focusWnd = GetFocusWindow();
    if (focusWnd == nullptr)
    {
        m_popup->ShowInfoDialog(DialogType::Alert, "There's no Main Window available to save.");
        return true;
    }

    wstring selectedFilename{};
    if (!m_popup->ShowFileDialog(selectedFilename, FileDialogType::Save))
        return false;

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
