#include "pch.h"
#include "MainMenuBar.h"
#include "Popup.h"
#include "MainWindow.h"
#include "ToolSystem.h"
#include "../Toy/Window.h"

using namespace Tool;

MainMenuBar::MainMenuBar(ToolSystem* toolSystem, Popup* popup) :
    m_toolSystem{ toolSystem },
    m_popup{ popup }
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
    if (ImGui::MenuItem("New", "Ctrl+N"))
    {
        HandleFileMenuAction(FileMenuAction::NewFile);
    }

    if (ImGui::MenuItem("Open", "Ctrl+O"))
    {
        HandleFileMenuAction(FileMenuAction::OpenFile);
    }

    ImGui::Separator();

    if (ImGui::MenuItem("Quit"))
    {
        HandleFileMenuAction(FileMenuAction::Quit);
    }
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
    case FileMenuAction::OpenFile:
        result = CreateMainWindowFromFile();
        break;
    case FileMenuAction::NewFile:
        // NewFile 관련 로직 추가
        break;
    case FileMenuAction::Quit:
        // Quit 관련 로직 추가
        Window::ExitGame();
        break;
    default:
       break;
    }
    m_currentAction.reset(); // 상태 초기화

    return result;
}

bool MainMenuBar::CreateMainWindowFromFile()
{
    std::wstring selectedFilename{};
    if (!m_popup->OpenFileDialog(selectedFilename))
        return false;

    auto mainWindow = std::make_shared<MainWindow>(m_toolSystem->GetRenderer());
    if (!mainWindow->CreateScene(selectedFilename))
    {
        m_popup->ShowErrorDialog("Failed to open the file. Please check the file path.");
        return true;
    }

    m_toolSystem->SetMainWindow(mainWindow);

    return true;
}