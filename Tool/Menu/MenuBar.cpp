#include "pch.h"
#include "MenuBar.h"
#include "FileTab.h"

using namespace Tool;

MenuBar::MenuBar(ToolSystem* toolSystem, Dialog* dialog) :
    m_fileTab{ make_unique<FileTab>(toolSystem, dialog) }
{}

MenuBar::~MenuBar() = default;

void MenuBar::Update() const
{
    m_fileTab->Excute();
}

template<typename T>
void Show(const string& menuName, const T& tab)
{
    if (!ImGui::BeginMenu(menuName.c_str())) return;
    tab->Show();
    ImGui::EndMenu();
}

bool MenuBar::Render() const
{
    if (!ImGui::BeginMainMenuBar()) 
        return true;

    Show("File", m_fileTab);

    ImGui::EndMainMenuBar();

    return true;
}
