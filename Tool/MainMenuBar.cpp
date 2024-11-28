#include "pch.h"
#include "MainMenuBar.h"
#include "Popup.h"
#include "MainWindow.h"
#include "ToolSystem.h"
#include "../Toy/Window.h"
#include "../Toy/UserInterface/JsonOperation.h"
#include "../Toy/UserInterface/JsonHelper.h"
#include "../Toy/Config.h"

class RecentFiles
{
    const size_t maxRecentFiles = 20;
    const size_t maxShownFiles = 5;    // "Open Recent" 메뉴에 표시할 파일 개수

public:
    RecentFiles()
    {
        JsonOperation jsonOp;
        jsonOp.Read(L"Tool/OpenRecentFiles.json");
        SerializeIO(jsonOp);
    }

    void AddFile(const std::string& filename)
    {
        auto it = ranges::find(m_recentFiles, filename);
        if (it != m_recentFiles.end()) {
            m_recentFiles.erase(it);
        }

        m_recentFiles.push_front(filename);
        if (m_recentFiles.size() > maxRecentFiles)
            m_recentFiles.pop_back();

        JsonOperation jsonOp;
        SerializeIO(jsonOp);
        jsonOp.Write(L"Tool/OpenRecentFiles.json"); 
    }
    
    void SerializeIO(JsonOperation& jsonOp)
    {
        jsonOp.Process("RecentFiles", m_recentFiles);
    }

    void ShowUI() {
        if (ImGui::BeginMenu("Open Recent")) {
            size_t shownCount = 0;
            for (const auto& file : m_recentFiles) {
                // 최대 표시 개수 제한
                if (shownCount >= maxShownFiles) break;

                if (ImGui::MenuItem(file.c_str())) {
                    //OpenFile(file);
                }
                shownCount++;
            }

            // "More.." 메뉴 표시
            if (m_recentFiles.size() > maxShownFiles) {
                if (ImGui::BeginMenu("More..")) {
                    for (size_t i = maxShownFiles; i < m_recentFiles.size(); ++i) {
                        if (ImGui::MenuItem(m_recentFiles[i].c_str())) {
                            //OpenFile(m_recentFiles[i]);
                        }
                    }
                    ImGui::EndMenu();
                }
            }

            ImGui::EndMenu();
        }
    }

private:
    deque<string> m_recentFiles;
};

///////////////////////////////////////////////////////

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
    if (ImGui::MenuItem("New", "Ctrl+N")) HandleFileMenuAction(FileMenuAction::NewFile);
    if (ImGui::MenuItem("Open", "Ctrl+O")) HandleFileMenuAction(FileMenuAction::OpenFile);
    m_recentFiles->ShowUI();
    //if (ImGui::BeginMenu("Open Recent"))
    //{
    //    if (ImGui::MenuItem("test1.json"))
    //    {
    //        int a = 1;
    //    }

    //    ImGui::EndMenu();
    //}

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
    case FileMenuAction::OpenFile: result = CreateMainWindowFromFile(); break;
    
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
    string filename = "test" + to_string(idx++) + ".json";
    m_recentFiles->AddFile(filename);
    return true;
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

bool MainMenuBar::OpenRecentFile()
{
    return true;
}