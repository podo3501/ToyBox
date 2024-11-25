#include "pch.h"
#include "MainMenuBar.h"
#include "Popup.h"
#include "MainWindow.h"
#include "ToolSystem.h"

using namespace Tool;

MainMenuBar::MainMenuBar(ToolSystem* toolSystem, Popup* popup) :
    m_toolSystem{ toolSystem },
    m_popup{ popup }
{}

MainMenuBar::~MainMenuBar() = default;

bool MainMenuBar::Excute()
{
    ShowMainMenuBar();
    return HandleFileMenu();
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
    m_currentAction = action; // ���� ������Ʈ
}

bool MainMenuBar::HandleFileMenu()
{
    if (!m_currentAction.has_value()) return true;

    switch (m_currentAction.value())
    {
    case FileMenuAction::OpenFile:
        m_currentAction.reset(); // ���� �ʱ�ȭ
        return CreateMainWindowFromFile();

    case FileMenuAction::NewFile:
        m_currentAction.reset();
        // NewFile ���� ���� �߰�
        break;

    case FileMenuAction::Quit:
        m_currentAction.reset();
        // Quit ���� ���� �߰�
        break;

    default:
        m_currentAction.reset();
        break;
    }

    return false;
}

bool MainMenuBar::CreateMainWindowFromFile()
{
    std::wstring selectedFilename{};
    if (!m_popup->OpenFileDialog(selectedFilename))
        return false;

    auto mainWindow = std::make_unique<MainWindow>(m_toolSystem->GetRenderer());
    if (!mainWindow->CreateScene(selectedFilename))
    {
        m_popup->ShowErrorDialog("Failed to open the file. Please check the file path.");
        return true;
    }

    m_toolSystem->SetMainWindow(move(mainWindow));

    return true;
}