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
    HandleFileMenu();   //������ �޴��� �����ϴ� ��
}

bool MainMenuBar::Excute()
{
    ShowMainMenuBar();  //� �޴��� �����ߴ���
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
    m_currentAction = action; // �޴��� �ѹ��� �ϳ��� Ŭ���ϱ� ������ ���°� �ϳ��� ����ȴ�.
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
        // NewFile ���� ���� �߰�
        break;
    case FileMenuAction::Quit:
        // Quit ���� ���� �߰�
        Window::ExitGame();
        break;
    default:
       break;
    }
    m_currentAction.reset(); // ���� �ʱ�ȭ

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