#include "pch.h"
#include "MainTextureWindow.h"
#include "EditFontTexture.h"
#include "EditSourceTexture.h"
#include "../Utility.h"
#include "../Toy/Utility.h"
#include "../Toy/UserInterface/Component/RenderTexture.h"
#include "../Toy/UserInterface/Component/ImageGrid1.h"
#include "../Toy/UserInterface/TextureSourceBinder/TextureSourceBinder.h"
#include "../Toy/InputManager.h"
#include "../Toy/UserInterface/UIUtility.h"

MainTextureWindow::~MainTextureWindow()
{
    m_renderer->RemoveImguiComponent(this);
}

MainTextureWindow::MainTextureWindow(IRenderer* renderer) :
    InnerWindow{ "empty" },
    m_renderer{ renderer },
    m_sourceTexture{ nullptr },
    m_editFontTexture{ make_unique<EditFontTexture>() },
    m_editSourceTexture{ make_unique<EditSourceTexture>(renderer, this) }
{
    m_renderer->AddImguiComponent(this);
}

bool MainTextureWindow::CreateNew()
{
    m_sourceBinder = CreateSourceBinder();
    ReturnIfFalse(m_sourceBinder);

    m_editFontTexture->SetSourceBinder(m_sourceBinder.get());
    m_editSourceTexture->SetSourceBinder(m_sourceBinder.get());
    m_isOpen = true;
    return true;
}

void MainTextureWindow::SetTexture(ImageGrid1* texture) noexcept 
{ 
    m_sourceTexture = texture;
    (texture) ? SetName(WStringToString(texture->GetFilename())) : SetName("empty");
}

bool MainTextureWindow::Create(const wstring& filename)
{
    m_sourceBinder = CreateSourceBinder(filename);
    ReturnIfFalse(m_sourceBinder);

    m_editFontTexture->SetSourceBinder(m_sourceBinder.get());
    m_editSourceTexture->SetSourceBinder(m_sourceBinder.get());
    m_isOpen = true;
    return true;
}

void MainTextureWindow::Update()
{
    if (!m_window) return;
    SetMouseStartOffset(m_window);

    m_editFontTexture->Update();
    m_editSourceTexture->Update();
}

ImVec2 MainTextureWindow::GetWindowSize() const noexcept
{
    if (!m_sourceTexture) return ImVec2{ 512, 512 };
    return XMUINT2ToImVec2(m_sourceTexture->GetSize());
}

void MainTextureWindow::Render(ImGuiIO* io)
{
    if (!m_isOpen) return;

    const ImVec2& size = GetWindowSize();
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::SetNextWindowSize({ size.x, size.y + GetFrameHeight() });
    ImGui::Begin(GetName().c_str(), &m_isOpen, ImGuiWindowFlags_AlwaysAutoResize);
    ImGui::PopStyleVar();   //������ ��Ÿ���� �����Ѵ�.

    if (ImGui::IsWindowAppearing())
        m_window = GetImGuiWindow();

    if (m_sourceTexture)
    {
        if (auto offset = m_sourceTexture->GetGraphicMemoryOffset(); offset != 0)
            ImGui::Image(offset, size);
    }

    IgnoreMouseClick(m_window);
    RenderSourceWindow();
    ImGui::End();
}

void MainTextureWindow::RenderSourceWindow()
{
    string wndName = string("Texture Source Window");
    ImGui::Begin(wndName.c_str(), nullptr, ImGuiWindowFlags_NoFocusOnAppearing);

    m_editFontTexture->Render();
    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();
    m_editSourceTexture->Render();

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