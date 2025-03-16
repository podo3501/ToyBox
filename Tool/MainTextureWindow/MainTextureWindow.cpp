#include "pch.h"
#include "MainTextureWindow.h"
#include "EditTextureWindow.h"
#include "../Utility.h"
#include "../Toy/Utility.h"
#include "../Toy/UserInterface/Component/RenderTexture.h"
#include "../Toy/UserInterface/Component/ImageGrid1.h"
#include "../Toy/InputManager.h"
#include "../Toy/UserInterface/UIUtility.h"

MainTextureWindow::~MainTextureWindow()
{
    m_renderer->RemoveImguiComponent(this);
}

MainTextureWindow::MainTextureWindow(IRenderer* renderer) :
    InnerWindow{ "empty" },
    m_renderer{ renderer },
    m_sourceTexture{ make_unique<ImageGrid1>() },
    m_editTextureWindow{ make_unique<EditTextureWindow>() }
{
    m_renderer->AddImguiComponent(this);
}

bool MainTextureWindow::CreateNew()
{
    m_sourceBinder = CreateSourceBinder();
    ReturnIfFalse(m_sourceBinder);

    m_editTextureWindow->SetSourceBinder(m_sourceBinder.get());
    m_isOpen = true;
    return true;
}

bool MainTextureWindow::LoadTexture(const wstring& filename)
{
    m_sourceTexture->SetFilenameToLoadInfo(filename);
    ReturnIfFalse(m_renderer->LoadComponent(m_sourceTexture.get()));
    if (const auto& areaList = m_sourceTexture->GetTextureAreaList(); areaList)
        m_editTextureWindow->SetTextureAreaList(*areaList);

    SetName(WStringToString(filename));
    return true;
}

bool MainTextureWindow::Create(const wstring& filename)
{
    m_sourceBinder = CreateSourceBinder(filename);
    ReturnIfFalse(m_sourceBinder);

    m_editTextureWindow->SetSourceBinder(m_sourceBinder.get());
    m_isOpen = true;
    return true;
}

void MainTextureWindow::Update()
{
    if (!m_window) return;
    SetMouseStartOffset(m_window);
    m_editTextureWindow->Update();
}

ImVec2 MainTextureWindow::GetWindowSize() const noexcept
{
    const auto& size = m_sourceTexture->GetSize();
    return size != XMUINT2{} ? XMUINT2ToImVec2(size) : ImVec2{ 512, 512 };
}

void MainTextureWindow::Render(ImGuiIO* io)
{
    if (!m_isOpen) return;

    const ImVec2& size = GetWindowSize();
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::SetNextWindowSize({ size.x, size.y + GetFrameHeight() });
    ImGui::Begin(GetName().c_str(), &m_isOpen, ImGuiWindowFlags_AlwaysAutoResize);
    ImGui::PopStyleVar();   //윈도우 스타일을 지정한다.

    if (ImGui::IsWindowAppearing())
    {
        m_window = GetImGuiWindow();
        m_editTextureWindow->SetTextureWindow(this);
    }

    if(auto offset = m_sourceTexture->GetGraphicMemoryOffset(); offset != 0)
        ImGui::Image(offset, size);

    IgnoreMouseClick(m_window);
    m_editTextureWindow->Render();

    ImGui::End();
}

bool MainTextureWindow::SaveScene(const wstring& filename)
{
    return m_sourceBinder->Save(filename);
}

wstring MainTextureWindow::GetSaveFilename() const noexcept
{
    return m_sourceBinder->GetJsonFilename();
}