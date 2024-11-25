#include "pch.h"
#include "MainMenuBar.h"
#include "Popup.h"
#include "MainWindow.h"

using namespace Tool;

MainMenuBar::MainMenuBar(Popup* popup) :
    m_popup{ popup }
{}

MainMenuBar::~MainMenuBar() = default;

unique_ptr<MainWindow> MainMenuBar::OpenFile(IRenderer* renderer) noexcept
{
    unique_ptr<MainWindow> mainWindow = nullptr;// make_unique<MainWindow>(renderer);

    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("Open", "Ctrl+O"))
            {
                wstring selectedFilename{};
                auto result = m_popup->OpenFileDialog(selectedFilename);
                if (!result) return nullptr;

                mainWindow = make_unique<MainWindow>(renderer);
                result = mainWindow->CreateScene(selectedFilename);
                if (result == false)
                {
                    m_popup->ShowErrorDialog("Failed to open the file. Please check the file path.");
                    mainWindow.reset();
                }
            }
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }

    return mainWindow;
}