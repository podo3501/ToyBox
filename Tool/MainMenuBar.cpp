#include "pch.h"
#include "MainMenuBar.h"
#include "Popup.h"
#include "MainWindow.h"
#include "ToolSystem.h"
#include "../Toy/Window.h"
#include "../Toy/UserInterface/JsonHelper.h"
#include "../Toy/Config.h"
#include "RecentFiles.h"

using namespace Tool;

MainMenuBar::MainMenuBar(ToolSystem* toolSystem, Popup* popup) :
    m_toolSystem{ toolSystem },
    m_popup{ popup },
    m_recentFiles{ make_unique<RecentFiles>() }
{}

MainMenuBar::~MainMenuBar() = default;

void MainMenuBar::Update()
{
    HandleFileMenu();   //선택한 메뉴를 실행하는 곳
}

bool MainMenuBar::Excute()
{
    ShowMainMenuBar();  //어떤 메뉴를 선택했는지
    return true;
}

void MainMenuBar::ShowMainMenuBar()
{
    if (!ImGui::BeginMainMenuBar()) return;

    if (ImGui::BeginMenu("File"))
    {
        RenderFileMenu();
        ImGui::EndMenu();
    }

    ImGui::EndMainMenuBar();
}

void MainMenuBar::RenderFileMenu()
{
    if (ImGui::MenuItem("New")) HandleFileMenuAction(FileMenuAction::NewFile);
    if (ImGui::MenuItem("Open", "Ctrl+O")) HandleFileMenuAction(FileMenuAction::OpenFile);
    if (m_recentFiles->ShowRecentFilesMenu()) HandleFileMenuAction(FileMenuAction::OpenRecent);
    if (ImGui::MenuItem("Save", "Ctrl+S")) HandleFileMenuAction(FileMenuAction::SaveFile);

    ImGui::Separator();

    if (ImGui::MenuItem("Quit")) HandleFileMenuAction(FileMenuAction::Quit);
}

void MainMenuBar::HandleFileMenuAction(FileMenuAction action)
{
    m_currentAction = action; // 메뉴는 한번에 하나씩 클릭하기 때문에 상태가 하나만 저장된다.
}

bool MainMenuBar::HandleFileMenu()
{
    if (!m_currentAction.has_value()) return true;

    auto result{ true };
    switch (m_currentAction.value())
    {
    case FileMenuAction::NewFile: result = NewFile();  break;
    case FileMenuAction::OpenFile: result = CreateMainWindow(); break;
    case FileMenuAction::OpenRecent: result = m_recentFiles->OpenFile(*this); break;
    case FileMenuAction::SaveFile: result = SaveMainWindow(); break;
    
    case FileMenuAction::Quit: Window::ExitGame(); break;
    default:
       break;
    }
    m_currentAction.reset(); // 상태 초기화

    return result;
}

bool MainMenuBar::NewFile() noexcept
{
    static auto idx{ 0 };
    wstring filename = L"test" + to_wstring(idx++) + L".json";
    m_recentFiles->AddFile(filename);
    return true;
}

bool MainMenuBar::CreateMainWindowFromFile(const wstring& filename)
{
    auto mainWindow = std::make_unique<MainWindow>(m_toolSystem->GetRenderer());
    if (!mainWindow->CreateScene(filename))
    {
        m_popup->ShowErrorDialog("Failed to open the file. Please check the file path.");
        return false;
    }

    m_toolSystem->SetMainWindow(move(mainWindow));

    return true;
}

bool MainMenuBar::CreateMainWindow()
{
    wstring selectedFilename{};
    if (!m_popup->OpenFileDialog(selectedFilename))
        return false;

    if (selectedFilename.empty())   //다이얼로그를 닫거나 취소를 눌리면 파일명이 없다.
        return true;

    CreateMainWindowFromFile(selectedFilename);
    m_recentFiles->AddFile(selectedFilename);

    return true;
}

bool MainMenuBar::SaveMainWindow()
{
    const MainWindow* focusWnd = m_toolSystem->GetFocusMainWindow();
    if (focusWnd == nullptr) return true;

    auto result = focusWnd->SaveScene();
    if (result)
    {
        //m_popup->ShowErrorDialog("Failed to save the file.");
        //Saved to a.json
    }
    else
        m_popup->ShowErrorDialog("Failed to save the file.");

    return result;
}